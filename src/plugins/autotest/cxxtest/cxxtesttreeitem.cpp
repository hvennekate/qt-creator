#include "cxxtesttreeitem.h"

namespace Autotest {
	namespace Internal {

		CxxTestTreeItem::CxxTestTreeItem(const QString &name, const QString &filePath, Type type)
			: TestTreeItem(name, filePath, type) {}

		QVariant CxxTestTreeItem::data(int column, int role) const
		{
			return QVariant();
		}

		TestTreeItem *CxxTestTreeItem::copyWithoutChildren()
		{
			return nullptr;
		}

		bool CxxTestTreeItem::canProvideTestConfiguration() const
		{
			return false;
		}

		bool CxxTestTreeItem::canProvideDebugConfiguration() const
		{
			return false;
		}

		TestConfiguration *CxxTestTreeItem::testConfiguration() const
		{
			return nullptr;
		}

		TestConfiguration *CxxTestTreeItem::debugConfiguration() const
		{
			return nullptr;
		}

		QList<TestConfiguration *> CxxTestTreeItem::getAllTestConfigurations() const
		{
			return QList<TestConfiguration*>();
		}

		QList<TestConfiguration *> CxxTestTreeItem::getSelectedTestConfigurations() const
		{
			return QList<TestConfiguration*>();
		}

		QList<TestConfiguration *> CxxTestTreeItem::getTestConfigurationsForFile(const Utils::FileName &fileName) const
		{
			return QList<TestConfiguration*>();
		}

		TestTreeItem *CxxTestTreeItem::find(const TestParseResult *result)
		{
			return nullptr;
		}

		TestTreeItem *CxxTestTreeItem::findChild(const TestTreeItem *other)
		{
			return nullptr;
		}

		bool CxxTestTreeItem::modify(const TestParseResult *result)
		{
			return false;
		}

		bool CxxTestTreeItem::isGroupNodeFor(const TestTreeItem *other) const
		{
			return false;
		}

		bool CxxTestTreeItem::isGroupable() const
		{
			return false;
		}

		TestTreeItem *CxxTestTreeItem::createParentGroupNode() const
		{
			return nullptr;
		}

		TestTreeItem *CxxTestTreeItem::applyFilters()
		{
			return nullptr;
		}

		QSet<QString> CxxTestTreeItem::internalTargets() const
		{
			return QSet<QString>();
		}
	} // namespace Internal
} // namespace Autotest
