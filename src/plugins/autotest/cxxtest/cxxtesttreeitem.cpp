/****************************************************************************
**
** Copyright (C) 2020 Hendrik Vennekate
** Contact: Hendrik.Vennekate@posteo.de
**
** This file extends Qt Creator to support a customized version of CxxTest.
** (see https://sourceforge.net/projects/cxxtest/)
**
** GNU General Public License Usage
** This file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

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
				case TestFunction:
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
			case TestFunction: {
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
			QList<TestConfiguration *> result;
			ProjectExplorer::Project *project = ProjectExplorer::SessionManager::startupProject();
			if (!project || type() != Root)
				return result;

			forFirstLevelChildren([&result](TestTreeItem *child) {
				testConfigsFromCheckState(child, result);
			});
			return result;
		}

		QList<TestConfiguration *> CxxTestTreeItem::getTestConfigurationsForFile(const Utils::FilePath &fileName) const
		{
			QList<TestConfiguration*> result;
			ProjectExplorer::Project *project = ProjectExplorer::SessionManager::startupProject();
			if (!project || type() != Root) return result;

			const QString &file = fileName.toString();
			forAllChildren([&result, &file] (TestTreeItem *item) {
				if (item->type() != TestFunction) return;
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

		TestTreeItem *findMatchingTestTreeItem(const CxxTestParseResult *result, const CxxTestTreeItem *parentItem) {
			return parentItem->findFirstLevelChild([&](const TestTreeItem *other) {
				return result->matches(other);
			});
		}

		TestTreeItem *CxxTestTreeItem::find(const TestParseResult *parseResult)
		{
			const CxxTestParseResult *result = dynamic_cast<const CxxTestParseResult*>(parseResult);
			if (!result) return nullptr;
			switch(type()) {
			case Root:
				if (parseResult->framework->grouping()) {
					const QString path = QFileInfo(result->fileName).absolutePath();
					for (int row = 0; row < childCount(); ++row) {
						TestTreeItem *group = childAt(row);
						if (group->filePath() != path)
							continue;
						if (auto groupChild = group->find(result))
							return groupChild; // TODO check!
					}
					return nullptr;
				}
			case GroupNode:
			case TestCase:
			case TestFunction:
			case TestDataFunction:
			case TestSpecialFunction:
				return findMatchingTestTreeItem(result, this);
			default:
				return nullptr;
			}
		}

		TestTreeItem *findMatchingTestTreeItem(const TestTreeItem *item, const CxxTestTreeItem *parentItem) {
			return parentItem->findFirstLevelChild([&](const TestTreeItem *other) {
				return item->name() == other->name()
				        && item->filePath() == other->filePath()
				        && item->type() == other->type()
				        && item->proFile() == other->proFile();
			});
		}

		TestTreeItem *CxxTestTreeItem::findChild(const TestTreeItem *other)
		{
			if (!other) return nullptr;
			switch(type()) {
			case Root:
			case GroupNode:
			case TestCase:
			case TestFunction:
			case TestDataFunction:
			case TestSpecialFunction:
				return findMatchingTestTreeItem(other, this);
			default:
				return nullptr;
			}
		}

		bool CxxTestTreeItem::modify(const TestParseResult *result)
		{
			if (!result) return false;
			switch(type()) {
			case TestCase:
				return modifyLineAndColumn(result);
			case TestFunction:
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
