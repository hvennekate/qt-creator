#include "cxxoutputreader.h"
#include "cxxtestconfiguration.h"

namespace Autotest {
	namespace Internal {

		CxxTestConfiguration::CxxTestConfiguration(const QString &test, const QString &method)
			: DebuggableTestConfiguration(), // TODO set mode properly
			  test(test),
			  method(method)
		{}

		TestOutputReader *CxxTestConfiguration::outputReader(const QFutureInterface<TestResultPtr> &fi, QProcess *app) const
		{
			return new CxxOutputReader(fi, app, buildDirectory(), projectFile());
		}

		QStringList CxxTestConfiguration::argumentsForTestRunner(QStringList * /* omitted */) const
		{
			if (testCases().empty()) return QStringList();
			return testCases().first().split(" ");
		}

		Utils::Environment CxxTestConfiguration::filteredEnvironment(const Utils::Environment &original) const
		{
			return original;
		}
	} // namespace Internal
} // namespace Autotest


