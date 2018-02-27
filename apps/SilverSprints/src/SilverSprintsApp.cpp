#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"

#if defined( CINDER_MAC )
#include "cinder/app/cocoa/PlatformCocoa.h"
#endif

#include "app/GFXMain.h"
#include "data/GFXGlobal.h"
#include "data/StateManager.h"
#include "data/Config.h"

//#define DEBUG_MODE 0

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace tools;
//using namespace sb;

using namespace gfx;

class SilverSprintApp : public App {
public:
	void setup() override;
	void resize() override;
	void keyDown(KeyEvent event) override;
	void update() override;
	void draw() override;
	void cleanup() override;

	void loadSettings();
	void writeSettings();
	gfx::GFXMainRef mGfxMain;
};

void SilverSprintApp::setup()
{
#if defined(DEBUG_MODE)
	fs::path logPath = getAppPath().string() + "/logs/SilverSprint.log";
	log::makeLogger<log::LoggerFile>(logPath, false);
	CI_LOG_I("Setting up log file at " << logPath);
	console() << "path " << logPath.string() << endl;
#endif

	auto sysLogger = log::makeLogger<log::LoggerSystem>();
	sysLogger->setLoggingLevel(log::LEVEL_VERBOSE);

	CI_LOG_I("Start program");

	gl::enableAlphaBlending();

	mGfxMain = GFXMainRef(new GFXMain());
	mGfxMain->setup();

	loadSettings();
}

void SilverSprintApp::loadSettings()
{
	CI_LOG_I("Load settings");
	fs::path targetPath = ci::app::getAppPath().parent_path() / fs::path("settings.cfg");
#if defined(CINDER_MAC)
	targetPath = ci::app::Platform::get()->expandPath("~/Library") / fs::path("SilverSprints/settings.cfg");
#elif defined(CINDER_MSW)
	targetPath = getDocumentsDirectory() / fs::path("SilverSprints/settings.cfg");
#endif

	if (fs::exists(targetPath)) {
		config().read(loadFile(targetPath));
		//        config().load(targetPath);

		gfx::GFXGlobal::getInstance()->currentRaceType = (gfx::RACE_TYPE)config().get("settings", "race_type", (int)RACE_TYPE_DISTANCE);
		Model::instance().setRaceLengthMeters(config().get("settings", "race_length_meters", 100));
		Model::instance().setRaceTimeSeconds(config().get("settings", "race_time", 60));
		Model::instance().setUseKph(config().get("settings", "race_kph", true));
		Model::instance().setRollerDiameterMm(config().get("settings", "roller_diameter_mm", 114.3));
		Model::instance().setNumRacers(config().get("settings", "num_racers", 114.3));
		Model::instance().setRaceLogging(config().get("settings", "log_races", false));

		setFullScreen(config().get("app", "fullscreen", false));
	}
}

void SilverSprintApp::writeSettings()
{
	config().set("settings", "race_type", (int)gfx::GFXGlobal::getInstance()->currentRaceType);
	config().set("settings", "race_length_meters", Model::instance().getRaceLengthMeters());
	config().set("settings", "race_time", Model::instance().getRaceTimeSeconds());
	config().set("settings", "race_kph", Model::instance().getUsesKph());
	config().set("settings", "roller_diameter_mm", Model::instance().getRollerDiameterMm());
	config().set("settings", "num_racers", Model::instance().getNumRacers());
	config().set("settings", "log_races", Model::instance().getRaceLogging());

	fs::path targetPath = ci::app::getAppPath().parent_path() / fs::path("settings.cfg");

#if defined(CINDER_MAC)
	targetPath = ci::app::Platform::get()->expandPath("~/Library") / fs::path("SilverSprints/settings.cfg");
#elif defined(CINDER_MSW)
	targetPath = getDocumentsDirectory() / fs::path("SilverSprints/settings.cfg");
#endif

	CI_LOG_I("Writing settings to " << targetPath);

	auto d = ci::DataTargetPath::createRef(targetPath);
	config().write(d);
}

void SilverSprintApp::cleanup()
{
	CI_LOG_I("Exiting");
	writeSettings();
}

void SilverSprintApp::resize()
{
	Rectf originalSize(0, 0, 1920, 1080);
	gfx::GFXGlobal::getInstance()->setScale(originalSize.getCenteredFit(getWindowBounds(), true));
}

void SilverSprintApp::keyDown(KeyEvent event)
{
	if (event.isAccelDown() || event.isControlDown()) {
		if (event.getChar() == 'f') {
			setFullScreen(!isFullScreen());
			config().set("app", "fullscreen", isFullScreen());
		}
		else if (event.getChar() == '1') {
			StateManager::getInstance()->changeAppState(APP_STATE::RACE);
		}
		else if (event.getChar() == '2') {
			StateManager::getInstance()->changeAppState(APP_STATE::ROSTER);
		}
		else if (event.getChar() == '3') {
			StateManager::getInstance()->changeAppState(APP_STATE::SETTINGS);
		}
		else if (event.getChar() == 'r') {
			mGfxMain->reloadShaders();
		}
	}
}

void SilverSprintApp::update()
{
	mGfxMain->update();
}

void SilverSprintApp::draw()
{
	gl::clear(Color(0, 0, 0));

	// clear out the window with black
	gl::clear(Color(0, 0, 0));

	gl::pushMatrices(); {
		Rectf originalSize(0, 0, 1920, 1080);
		Rectf scaledFit = originalSize.getCenteredFit(getWindowBounds(), true);

		float hScale = getWindowWidth() / originalSize.getWidth();
		float vScale = getWindowHeight() / originalSize.getHeight();

		float scaleAmt = (hScale < vScale) ? hScale : vScale;

		gl::translate(vec2(scaledFit.x1, 0));
		gl::scale(scaleAmt, scaleAmt);

		mGfxMain->draw(scaledFit);
	}gl::popMatrices();

#if defined(DEBUG_MODE)
	gl::drawString(to_string(getAverageFps()), vec2(10, getWindowHeight() - 60));
	gl::drawString(StateManager::getInstance()->getCurrentAppStateString(), vec2(10, getWindowHeight() - 40));
	gl::drawString(StateManager::getInstance()->getCurrentRaceStateString(), vec2(10, getWindowHeight() - 20));
#endif
}

CINDER_APP(SilverSprintApp, RendererGl(RendererGl::Options().msaa(8)), [&](SilverSprintApp::Settings *settings) {
	settings->setFrameRate(60.0);
	settings->setWindowSize(1280, 700);
	//  settings->setWindowPos((1400-1280)/2, 0 );
	settings->setTitle("SilverSprint");
})
