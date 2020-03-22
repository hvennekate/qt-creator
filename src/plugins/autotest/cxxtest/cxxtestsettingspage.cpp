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

#include "cxxtestsettingspage.h"
#include "autotest/autotestconstants.h"
#include "cxxtestconstants.h"
#include "QCoreApplication"

namespace Autotest {
	namespace Internal {

		class CxxTestSettingsPageWidget : public Core::IOptionsPageWidget
		{
		public:
			CxxTestSettingsPageWidget(CxxTestSettings* settings) { Q_UNUSED(settings) }
			void apply() override;
		};

		void CxxTestSettingsPageWidget::apply()
		{
			// TODO apply settings
		}

		CxxTestSettingsPage::CxxTestSettingsPage(CxxTestSettings *settings, Core::Id settingsId)
		{
			setId(settingsId);
			setCategory(Constants::AUTOTEST_SETTINGS_CATEGORY);
			setDisplayName(QCoreApplication::translate(CxxTest::Constants::FRAMEWORK_NAME,
			                                           CxxTest::Constants::FRAMEWORK_SETTINGS_CATEGORY));
			setWidgetCreator([settings] { return new CxxTestSettingsPageWidget(settings); });
		}


	} // namespace Internal
} // namespace Autotest
