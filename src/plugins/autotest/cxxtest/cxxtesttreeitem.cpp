#include "cxxtestconfiguration.h"
#include "cxxtesttreeitem.h"
#include "cxxtestparser.h"
#include "../testframeworkmanager.h"
#include <utils/qtcassert.h>
#include <projectexplorer/session.h>

namespace Autotest {
	namespace Internal {

		CxxTestTreeItem::CxxTestTreeItem(const QString &name, const QString &filePath, Type type)
			: TestTreeItem(name, filePath, type) {}

		QVariant CxxTestTreeItem::data(int column, int role) const
		{
			switch (role) {
			case Qt::DisplayRole:
				if (type() == TestTreeItem::Root) break;
				return name();
			case Qt::DecorationRole:
				break;
			case Qt::ToolTipRole:
				return filePath();
			case Qt::CheckStateRole:
				switch (type()) {
				case Root:
				case GroupNode:
				case TestCase:
				case TestFunctionOrSet:
					return checked();
				default:
					return QVariant();
				}
			case ItalicRole:
				return false;
			case EnabledRole:
				return true;
			default:
				break;
			}
			return TestTreeItem::data(column, role);
		}

		TestTreeItem *CxxTestTreeItem::copyWithoutChildren()
		{
			auto copy = new CxxTestTreeItem;
			copy->copyBasicDataFrom(this);
			return copy;
		}

		bool CxxTestTreeItem::canProvideTestConfiguration() const
		{
			return type() != Root;
		}

		bool CxxTestTreeItem::canProvideDebugConfiguration() const
		{
			return type() != Root;
		}

		TestConfiguration *CxxTestTreeItem::testConfiguration() const
		{
			ProjectExplorer::Project *project = ProjectExplorer::SessionManager::startupProject();
			QTC_ASSERT(project, return nullptr);

			CxxTestConfiguration *config = nullptr;
			switch (type()) {
			case Root:
				if (int count = childCount()) {
					config = new CxxTestConfiguration(name());
					config->setTestCaseCount(count);
					config->setProjectFile(proFile());
					config->setProject(project);
				}
			case TestCase: {
			    if (int count = childCount()) {
				config = new CxxTestConfiguration(name());
				config->setTestCases(QStringList(name()));
				config->setTestCaseCount(count);
				config->setProjectFile(proFile());
				config->setProject(project);
			    }
			    break;
			}
			case TestFunctionOrSet: {
			    CxxTestTreeItem *parent = dynamic_cast<CxxTestTreeItem *>(parentItem());
			    if (!parent)
				return nullptr;
			    config = new CxxTestConfiguration(parent->name(), name());
			    config->setTestCases(QStringList(parent->name() + " " + name()));
			    config->setProjectFile(proFile());
			    config->setProject(project);
			    break;
			}
			default:
			    return nullptr;
			}
			if (config)
			    config->setInternalTargets(internalTargets());
			return config;
		}

		TestConfiguration *CxxTestTreeItem::debugConfiguration() const
		{
			auto config = dynamic_cast<DebuggableTestConfiguration*>(testConfiguration());
			if (config) config->setRunMode(TestRunMode::Debug);
			return config;
		}

		QList<TestConfiguration *> CxxTestTreeItem::getAllTestConfigurations() const
		{
			QList<TestConfiguration*> result;
			ProjectExplorer::Project *project = ProjectExplorer::SessionManager::startupProject();
			if (!project || type() != Root)
			    return result;

			forFirstLevelChildren([&result](TestTreeItem *child) {
				if (child->type() == TestCase) {
					if (auto configuration = child->testConfiguration())
						result << configuration;
				} else if (child->type() == GroupNode) {
					child->forFirstLevelChildren([&result](TestTreeItem *groupChild) {
						if (auto configuration = groupChild->testConfiguration())
							result << configuration;
					});
				}
			});
			return result;
		}

		void testConfigsFromCheckState(const TestTreeItem *item,
					       QList<TestConfiguration*> &configs) {
			qDebug() << "Getting configs from check state" << item << item->type();
			if (!item) return;
			if (item->type() == TestTreeItem::TestCase) {
				switch (item->checked()) {
				case Qt::Unchecked: return;
				case Qt::Checked:
					configs << item->testConfiguration();
					return;
				case Qt::PartiallyChecked:
					item->forFirstLevelChildren([&configs] (const TestTreeItem* child) { testConfigsFromCheckState(child, configs);});
					return;
				}
			}
			if (item->type() == TestTreeItem::GroupNode) {
				item->forFirstLevelChildren([&configs] (TestTreeItem* child) { testConfigsFromCheckState(child, configs); });
				return;
			}

			if (item->checked() == Qt::Unchecked) return;
			if (item->checked() == Qt::Checked) {
				if (auto config = dynamic_cast<CxxTestConfiguration*>(item->testConfiguration()))
					configs << config;
				return;
			}
			if (item->checked() == Qt::PartiallyChecked) {
				QStringList testCases;
				item->forFirstLevelChildren([&testCases](TestTreeItem *grandChild) {
					if (grandChild->checked() == Qt::Checked) {
						testCases << grandChild->name();
					} else if (grandChild->checked() == Qt::PartiallyChecked) {
						const QString prefix = grandChild->name() + ":";
						grandChild->forFirstLevelChildren([&testCases, &prefix](TestTreeItem *testFunction) {
							if (testFunction->checked() == Qt::Checked)
								testCases << prefix + testFunction->name();
						});
					}
				});

				auto config = new CxxTestConfiguration;
				config->setTestCases(testCases);
				config->setProjectFile(item->proFile());
				config->setProject(ProjectExplorer::SessionManager::startupProject());
				config->setInternalTargets(item->internalTargets());
				configs << config;
			}
		}

		QList<TestConfiguration *> CxxTestTreeItem::getSelectedTestConfigurations() const
		{
			qDebug() << "Getting selected test configurations";
			QList<TestConfiguration *> result;
			ProjectExplorer::Project *project = ProjectExplorer::SessionManager::startupProject();
			if (!project || type() != Root)
			    return result;

			qDebug() << "traversing items to find checked tests";
			forFirstLevelChildren([&result](TestTreeItem *child) {
				testConfigsFromCheckState(child, result);
			});
			return result;
		}

		QList<TestConfiguration *> CxxTestTreeItem::getTestConfigurationsForFile(const Utils::FileName &fileName) const
		{
			QList<TestConfiguration*> result;
			ProjectExplorer::Project *project = ProjectExplorer::SessionManager::startupProject();
			if (!project || type() != Root) return result;

			const QString &file = fileName.toString();
			forAllChildren([&result, &file] (TestTreeItem *item) {
				if (item->type() != TestFunctionOrSet) return;
				if (item->filePath() != file) return;
				auto testCase = item->parentItem();
				if (!testCase) return;
				if (testCase->type() != TestCase) return;
				auto config = testCase->testConfiguration();
				if (!config) return;
				config->setTestCases(QStringList() << item->name());
				result << config;
			});
			return result;
		}

		TestTreeItem *CxxTestTreeItem::find(const TestParseResult *result)
		{
			if (!result) return nullptr;
			switch(type()) {
			case Root:
				if (TestFrameworkManager::instance()->groupingEnabled(result->frameworkId)) {
				    const QString path = QFileInfo(result->fileName).absolutePath();
				    for (int row = 0; row < childCount(); ++row) {
					TestTreeItem *group = childAt(row);
					if (group->filePath() != path)
					    continue;
					if (auto groupChild = group->findChildByFile(result->fileName))
					    return groupChild;
				    }
				    return nullptr;
				}
				return findChildByFile(result->fileName);
			case GroupNode:
				return findChildByFile(result->fileName);
			case TestCase:
				return findChildByName(result->displayName);
			case TestFunctionOrSet:
			case TestDataFunction:
			case TestSpecialFunction:
				return findChildByName(result->name);
			default:
				return nullptr;
			}
		}

		TestTreeItem *CxxTestTreeItem::findChild(const TestTreeItem *other)
		{
			if (!other) return nullptr;
			switch(type()) {
			case Root:
				return findChildByFileAndType(other->filePath(), other->type());
			case GroupNode:
				return other->type() == TestCase ? findChildByFile(other->filePath()) : nullptr;
			case TestCase:
				if (other->type() != TestFunctionOrSet && other->type() != TestDataFunction && other->type() != TestSpecialFunction)
					return nullptr;
				return findChildByName(other->filePath());
			case TestFunctionOrSet:
			case TestDataFunction:
			case TestSpecialFunction:
				return other->type() == TestDataTag ? findChildByName(other->name()) : nullptr;
			default:
				return nullptr;
			}
		}

		bool CxxTestTreeItem::modify(const TestParseResult *result)
		{
			if (!result) return false;
			switch(type()) {
			case TestCase:
				return modifyTestCaseContent(result);
			case TestFunctionOrSet:
			case TestDataFunction:
			case TestSpecialFunction:
				return modifyTestFunctionContent(result);
			case TestDataTag:
				return modifyDataTagContent(result);
			default:
				return false;
			}
		}

		bool CxxTestTreeItem::isGroupable() const
		{
			return type() == TestCase;
		}

		TestTreeItem *CxxTestTreeItem::createParentGroupNode() const
		{
			const QFileInfo absolutePath(QFileInfo(filePath()).absolutePath());
			return new CxxTestTreeItem(absolutePath.baseName(), absolutePath.absolutePath(), TestTreeItem::GroupNode);
		}
	} // namespace Internal
} // namespace Autotest
