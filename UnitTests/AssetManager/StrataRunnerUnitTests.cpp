#include "StrataRunnerUnitTests.hpp"


BOOST_FIXTURE_TEST_SUITE(StrataRunnerTests, StrataRunnerInstance)

BOOST_AUTO_TEST_CASE(SpawnThreadsGOOD)
{
	StrataRunner runner;
	runner.SetConfiguration(myConfiguration);
	runner.SpawnThreads();
	runner.JoinThreads();
	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(ProcessPrerequisites)
{
	StrataRunner runner;
	runner.SetConfiguration(myConfiguration);
	runner.ProcessPrerequisites();
	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END()
