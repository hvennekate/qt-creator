#include "cxxtestparser.h"
#include "cxxtesttreeitem.h"
#include "cpptools/projectpart.h"
#include "cpptools/cppmodelmanager.h"
#include <QDebug>

namespace Autotest {
	namespace Internal {
		CxxTestParseResult::CxxTestParseResult(const Core::Id &id)
			: TestParseResult(id) {}

		TestTreeItem *CxxTestParseResult::createTestTreeItem() const
		{
			if (itemType != TestTreeItem::TestCase && itemType != TestTreeItem::TestFunctionOrSet) return nullptr;
			auto item = new CxxTestTreeItem(name, fileName, itemType);
			item->setProFile(proFile);
			item->setLine(line);
			item->setColumn(column);
			for (auto testFunction : children)
				item->appendChild(testFunction->createTestTreeItem());
			return item;
		}

		bool includesTestSuite(const CPlusPlus::Document::Ptr &document) {
			for (auto include : document->resolvedIncludes())
				if (include.resolvedFileName().endsWith("cxxtest/TestSuite.h"))
					return true;
			return false;
		}

		bool CxxTestParser::processDocument(QFutureInterface<TestParseResultPtr> futureInterface, const QString &fileName)
		{
//			qDebug() << "called \'processDocument\'" << fileName;
			if (!m_cppSnapshot.contains(fileName) || !selectedForBuilding(fileName)) return false;

			CPlusPlus::Document::Ptr document = m_cppSnapshot.find(fileName).value();
			if (!includesTestSuite(document)) return false;

			qDebug() << "promising document: " << fileName << document->globalSymbolCount();
			QVector<CPlusPlus::Function*> testFunctions;
			for (int i = 0; i < document->globalSymbolCount(); ++i) {
				CPlusPlus::Symbol *symbol = document->globalSymbolAt(i);
				qDebug() << "  Symbol: " << symbol->identifier()->chars() << "class?" << symbol->isClass();
				if (!symbol->isClass()) continue;
				qDebug() << "    Base classes: " << symbol->asClass()->baseClassCount();
				bool isTestSuite = false;
				for (int j = 0; j < symbol->asClass()->baseClassCount(); ++j) {
					CPlusPlus::BaseClass *base = symbol->asClass()->baseClassAt(j); // need to traverse higher
					qDebug() << "      " << base->identifier()->chars() << base->name()->identifier()->chars();
					isTestSuite |= (QString::fromUtf8(base->identifier()->chars()) == "TestSuite");
					if (base->enclosingNamespace()) // somehow, TestSuite's namespace does not appear to get picked up
						qDebug() << base->enclosingNamespace()->identifier()->chars();
				}
				if (!isTestSuite) continue;

				QString proFile;
				const QList<CppTools::ProjectPart::Ptr> &ppList = CppTools::CppModelManager::instance()->projectPart(fileName);
				if (ppList.size())
				    proFile = ppList.first()->projectFile;
				else
				    return false;
				auto classParseResult = new CxxTestParseResult(id());
				classParseResult->itemType = TestTreeItem::TestCase;
				classParseResult->fileName = fileName;
				classParseResult->name = QString::fromUtf8(symbol->identifier()->chars());
				classParseResult->line = symbol->line();
				classParseResult->column = symbol->column() - 1;
				classParseResult->proFile = proFile;

				for (int j = 0; j < symbol->asClass()->memberCount(); ++j) {
					CPlusPlus::Symbol *member = symbol->asClass()->memberAt(j);
					if (!member->isFunction() || !member->isPublic()) continue;
					const CPlusPlus::Identifier *identifier = symbol->asClass()->memberAt(j)->identifier();
					if (!identifier || !QString::fromUtf8(identifier->chars()).startsWith("test")) continue;
					qDebug() << "      " << identifier->chars() << symbol->asClass()->memberAt(j)->isFunction();
					testFunctions << member->asFunction();

					auto testMethod = new CxxTestParseResult(id());
					testMethod->name = QString::fromUtf8(member->identifier()->chars());
					testMethod->fileName = fileName;
					testMethod->line = member->line();
					testMethod->column = member->column() - 1;
					testMethod->itemType = TestTreeItem::TestFunctionOrSet;
					testMethod->proFile = proFile;
					classParseResult->children.append(testMethod);
				}

				futureInterface.reportResult(TestParseResultPtr(classParseResult));
			}

			return false;
		}
	} // namespace Internal
} // namespace Autotest
