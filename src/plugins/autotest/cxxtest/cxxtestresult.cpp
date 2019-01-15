#include "cxxtestresult.h"
#include "../testframeworkmanager.h"
#include "../testtreeitem.h"
#include "cxxtestconstants.h"
#include <QDebug>

namespace Autotest {
	namespace Internal {

		CxxTestResult::CxxTestResult(const QString &id, const QString &world, const QString &suite, const QString &test, const TestResult *parent)
			: TestResult(id, world),
			  parent(parent),
			  suite(suite),
			  test(test)
		{}

		const QString CxxTestResult::outputString(bool selected) const
		{
			return description(); // TODO
		}

		const TestTreeItem *CxxTestResult::findTestTreeItem() const
		{
			if (suite.isEmpty()) return nullptr;
			auto id = Core::Id(Constants::FRAMEWORK_PREFIX).withSuffix(CxxTest::Constants::FRAMEWORK_NAME);
			auto rootNode = TestFrameworkManager::instance()->rootNodeForTestFramework(id);
			qDebug() << "Root node: " << rootNode;
			if (!rootNode) return nullptr;
			const auto suiteNode = rootNode->findAnyChild([this](const Utils::TreeItem *item) {
				const auto treeItem = dynamic_cast<const TestTreeItem*>(item);
				return treeItem && treeItem->name() == suite;
			});
			qDebug() << suiteNode;
			if (test.isEmpty()) return  dynamic_cast<const TestTreeItem*>(suiteNode);
			return  dynamic_cast<const TestTreeItem*>(suiteNode->findAnyChild([this](const Utils::TreeItem *item) {
				const auto treeItem = dynamic_cast<const TestTreeItem*>(item);
				return treeItem && treeItem->name() == test;
			}));
		}

		bool CxxTestResult::isDirectParentOf(const TestResult *other, bool *needsIntermediate) const
		{
			auto cxxTestResult = dynamic_cast<const CxxTestResult*>(other);
			if (!cxxTestResult) return false;
			qDebug() << "isDirectParentOf" << other->description() << description() << cxxTestResult << cxxTestResult->parent << this;
			return cxxTestResult->parent == this;
		}
	} // namespace Internal
} // namespace Autotest
