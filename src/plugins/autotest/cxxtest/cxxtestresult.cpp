#include "cxxtestresult.h"
#include "../testframeworkmanager.h"
#include "../testtreeitem.h"
#include "cxxtestconstants.h"

namespace Autotest {
	namespace Internal {

		CxxTestResult::CxxTestResult(const QString &id, const QString &world, const QString &suite, const QString &test, const TestResult *parent)
		    : TestResult(id, world),
		      test(test),
		      suite(suite),
		      parent(parent)
		{}

		const QString CxxTestResult::outputString(bool selected) const
		{
			Q_UNUSED(selected)
			return description(); // TODO
		}

		const TestTreeItem *CxxTestResult::findTestTreeItem() const
		{
			if (suite.isEmpty()) return nullptr;
			auto id = Core::Id(Constants::FRAMEWORK_PREFIX).withSuffix(CxxTest::Constants::FRAMEWORK_NAME);
			auto rootNode = TestFrameworkManager::frameworkForId(id)->rootNode();
			if (!rootNode) return nullptr;
			const auto suiteNode = rootNode->findAnyChild([this](const Utils::TreeItem *item) {
				const auto treeItem = dynamic_cast<const TestTreeItem*>(item);
				return treeItem && treeItem->name() == suite;
			});
			if (test.isEmpty()) return  dynamic_cast<const TestTreeItem*>(suiteNode);
			return  dynamic_cast<const TestTreeItem*>(suiteNode->findAnyChild([this](const Utils::TreeItem *item) {
				const auto treeItem = dynamic_cast<const TestTreeItem*>(item);
				return treeItem && treeItem->name() == test;
			}));
		}

		bool CxxTestResult::isDirectParentOf(const TestResult *other, bool * /*needsIntermediate*/) const
		{
			auto cxxTestResult = dynamic_cast<const CxxTestResult*>(other);
			if (!cxxTestResult) return false;
			return cxxTestResult->parent == this;
		}
	} // namespace Internal
} // namespace Autotest
