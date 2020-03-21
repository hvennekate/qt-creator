#include "cxxtestparser.h"
#include "cxxtesttreeitem.h"
#include "cpptools/projectpart.h"
#include "cpptools/cppmodelmanager.h"

namespace Autotest {
	namespace Internal {
		CxxTestParseResult::CxxTestParseResult(ITestFramework *fw)
		    : TestParseResult(fw) {}

		TestTreeItem *CxxTestParseResult::createTestTreeItem() const
		{
			if (itemType != TestTreeItem::TestCase && itemType != TestTreeItem::TestFunction) return nullptr;
			auto item = new CxxTestTreeItem(name, fileName, itemType);
			item->setProFile(proFile);
			item->setLine(line);
			item->setColumn(column);
			for (auto testFunction : children)
				item->appendChild(testFunction->createTestTreeItem());
			return item;
		}

		bool CxxTestParseResult::matches(const TestTreeItem *item) const {
			return item->name() == name
			        && item->filePath() == fileName
			        && item->type() == itemType
			        && item->proFile() == proFile;
		}

		QSet<QString> CxxTestParser::parentTestClasses(const QString& fileName, QSet<QString> visitedFileNames) const
		{
			if (fileName.endsWith("cxxtest/TestSuite.h")) return {"TestSuite"}; // TODO namespace
			if (visitedFileNames.contains(fileName)) return {};
			visitedFileNames << fileName;
			if (!m_cppSnapshot.contains(fileName) || !selectedForBuilding(fileName)) return {};
			auto document = m_cppSnapshot.find(fileName).value();
			QSet<QString> parentClasses;
			for (auto include : document->resolvedIncludes())
				parentClasses += parentTestClasses(include.resolvedFileName(), visitedFileNames);
			for (int symbolIndex = 0; symbolIndex < document->globalSymbolCount(); ++symbolIndex) {
				CPlusPlus::Symbol *symbol = document->globalSymbolAt(symbolIndex);
				if (symbol->isTemplate()) symbol = symbol->asTemplate()->declaration();
				if (!symbol || !symbol->isClass()) continue;
				for (int baseIndex = 0; baseIndex < symbol->asClass()->baseClassCount(); ++baseIndex){
					auto baseClassName = QString::fromUtf8(symbol->asClass()->baseClassAt(baseIndex)->identifier()->chars());// TODO use name instead
					if (parentClasses.contains(baseClassName)) {
						parentClasses += QString::fromUtf8(symbol->identifier()->chars());
						break;
					}
				}
			}
			return parentClasses;
		}

		CxxTestParseResult *CxxTestParser::prepareTestParseResult(const QString &fileName, const QString &proFile, CPlusPlus::Symbol *symbol, TestTreeItem::Type type)
		{
			auto classParseResult = new CxxTestParseResult(framework());
			classParseResult->itemType = type;
			classParseResult->fileName = fileName;
			classParseResult->name = QString::fromUtf8(symbol->identifier()->chars());
			classParseResult->line = symbol->line();
			classParseResult->column = symbol->column() - 1;
			classParseResult->proFile = proFile;

			return classParseResult;
		}

		CxxTestParseResult *CxxTestParser::generateClassParseResult(CPlusPlus::Symbol *symbol, QString proFile, const QString &fileName)
		{
			auto classParseResult = prepareTestParseResult(fileName, proFile, symbol, TestTreeItem::TestCase);

			for (int j = 0; j < symbol->asClass()->memberCount(); ++j) {
				CPlusPlus::Symbol *member = symbol->asClass()->memberAt(j);
				if (!member->isFunction() || !member->isPublic()) continue;
				const CPlusPlus::Identifier *identifier = symbol->asClass()->memberAt(j)->identifier();
				if (!identifier || !QString::fromUtf8(identifier->chars()).startsWith("test")) continue;

				classParseResult->children.append(prepareTestParseResult(fileName, proFile, member, TestTreeItem::TestFunction));
			}

			return classParseResult;
		}

		bool CxxTestParser::processDocument(QFutureInterface<TestParseResultPtr> futureInterface, const QString &fileName)
		{
			if (!m_cppSnapshot.contains(fileName) || !selectedForBuilding(fileName)) return false;
			QString proFile;
			const QList<CppTools::ProjectPart::Ptr> &ppList = CppTools::CppModelManager::instance()->projectPart(fileName);
			if (ppList.isEmpty()) return false;
			else proFile = ppList.first()->projectFile;

			CPlusPlus::Document::Ptr document = m_cppSnapshot.find(fileName).value();
			auto testClasses = parentTestClasses(fileName, {});
			if (testClasses.isEmpty()) return false;

			for (int i = 0; i < document->globalSymbolCount(); ++i) {
				CPlusPlus::Symbol *symbol = document->globalSymbolAt(i);
				if (!symbol->identifier()) continue;
				if (!symbol->isClass() || !testClasses.contains(QString::fromUtf8(symbol->identifier()->chars()))) continue;

				futureInterface.reportResult(TestParseResultPtr(generateClassParseResult(symbol, proFile, fileName)));
			}

			return false;
		}
	} // namespace Internal
} // namespace Autotest
