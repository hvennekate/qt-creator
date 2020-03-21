#include "cxxtestframework.h"

#include "cxxtestconstants.h"
#include "cxxtestparser.h"
#include "cxxtestsettings.h"
#include "cxxtestsettingspage.h"
#include "cxxtesttreeitem.h"

namespace Autotest {
	namespace Internal {
		CxxTestFramework::CxxTestFramework()
		    : ITestFramework(true),
		      m_settings(new CxxTestSettings),
		      m_settingsPage(new CxxTestSettingsPage(m_settings.get(), settingsId()))
		{}

		const char *CxxTestFramework::name() const
		{
			return CxxTest::Constants::FRAMEWORK_NAME;
		}

		unsigned CxxTestFramework::priority() const
		{
			return CxxTest::Constants::FRAMEWORK_PRIORITY;
		}

		IFrameworkSettings *CxxTestFramework::frameworkSettings()
		{
			return new CxxTestSettings;
		}

		QString CxxTestFramework::groupingToolTip() const
		{
			return QCoreApplication::translate("CxxTestFramework",
			                                   "See also CxxTest settings");
		}

		ITestParser *CxxTestFramework::createTestParser()
		{
			return new CxxTestParser(this);
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
