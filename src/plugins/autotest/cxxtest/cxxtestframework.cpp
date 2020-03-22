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
