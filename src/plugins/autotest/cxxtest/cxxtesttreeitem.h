#ifndef CXXTESTTREEITEM_H
#define CXXTESTTREEITEM_H

#include <autotest/testtreeitem.h>

namespace Autotest {
	namespace Internal {

		class CxxTestTreeItem : public TestTreeItem
		{
		public:
			CxxTestTreeItem(const QString &name = QString(), const QString &filePath = QString(),
					Type type = Root);
			QVariant data(int column, int role) const override;
			TestTreeItem *copyWithoutChildren() override;
			bool canProvideTestConfiguration() const override;
			bool canProvideDebugConfiguration() const override;
			TestConfiguration *testConfiguration() const override;
			TestConfiguration *debugConfiguration() const override;
			QList<TestConfiguration *> getAllTestConfigurations() const override;
			QList<TestConfiguration *> getSelectedTestConfigurations() const override;
			QList<TestConfiguration *> getTestConfigurationsForFile(const Utils::FileName &fileName) const override;
			TestTreeItem *find(const TestParseResult *result) override;
			TestTreeItem *findChild(const TestTreeItem *other) override;
			bool modify(const TestParseResult *result) override;
			bool isGroupNodeFor(const TestTreeItem *other) const override;
			bool isGroupable() const override;
			TestTreeItem *createParentGroupNode() const override;
			TestTreeItem *applyFilters() override;
			QSet<QString> internalTargets() const override;
		};

	} // namespace Internal
} // namespace Autotest

#endif // CXXTESTTREEITEM_H
