#include "cxxtestresult.h"
#include "../testframeworkmanager.h"
#include "../testtreeitem.h"
#include "cxxtestconstants.h"
#include <QDebug>

namespace Autotest {
	namespace Internal {

		CxxTestResult::CxxTestResult(const QString &id, const QString &name, const TestResult *parent)
			: TestResult(id, name),
			  parent(parent)
		{}

		CxxTestResult::CxxTestResult(const QString &id, const QString &method, const QString &testClass, const TestResult *parent)
			: TestResult(id, method),
			  parent(parent)
		{}

		const QString CxxTestResult::outputString(bool selected) const
		{
			return description(); // TODO
		}

		const TestTreeItem *CxxTestResult::findTestTreeItem() const
		{
			if (!parent) return nullptr;
			auto id = Core::Id(Constants::FRAMEWORK_PREFIX).withSuffix(CxxTest::Constants::FRAMEWORK_NAME);
			auto rootNode = TestFrameworkManager::instance()->rootNodeForTestFramework(id);
			qDebug() << "Root node: " << rootNode;
			if (!rootNode) return nullptr;
			const auto item = rootNode->findAnyChild([this](const Utils::TreeItem *item) {
				const auto treeItem = dynamic_cast<const TestTreeItem*>(item);
				return treeItem && treeItem->name() == name()
						&& (!parent
						|| (treeItem->parentItem()
						    && treeItem->parentItem()->name() == parent->description()));
			});
			qDebug() << item;
			return dynamic_cast<const TestTreeItem*>(item);
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
