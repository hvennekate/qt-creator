#include "cxxtestframework.h"

#include "cxxtestconstants.h"
#include "cxxtestparser.h"
#include "cxxtestsettings.h"
#include "cxxtestsettingspage.h"
#include "cxxtesttreeitem.h"

namespace Autotest {
	namespace Internal {
		const char *CxxTestFramework::name() const
		{
			return CxxTest::Constants::FRAMEWORK_NAME;
		}

		unsigned CxxTestFramework::priority() const
		{
			return CxxTest::Constants::FRAMEWORK_PRIORITY;
		}

		bool CxxTestFramework::hasFrameworkSettings() const
		{
			return true;
		}

		IFrameworkSettings *CxxTestFramework::createFrameworkSettings() const
		{
			return new CxxTestSettings;
		}

		ITestSettingsPage *CxxTestFramework::createSettingsPage(QSharedPointer<IFrameworkSettings> settings) const
		{
			return new CxxTestSettingsPage(settings, this);
		}

		QString CxxTestFramework::groupingToolTip() const
		{
			return QCoreApplication::translate("CxxTestFramework",
							   "See also CxxTest settings");
		}

		ITestParser *CxxTestFramework::createTestParser() const
		{
			return new CxxTestParser;
		}

		TestTreeItem *CxxTestFramework::createRootNode() const
		{
			return new CxxTestTreeItem(
						QCoreApplication::translate("CxxTestFramework",
									    CxxTest::Constants::FRAMEWORK_SETTINGS_CATEGORY),
						QString(), TestTreeItem::Root);
		}
	} // namespace Internal
} // namespace Autotest
