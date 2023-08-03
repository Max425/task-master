#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WServer.h>
#include <Wt/Dbo/SqlConnectionPool.h>
#include <Wt/WJavaScriptPreamble.h>

#include "Session.h"
#include "MainView.h"
#include "DbDriver.hpp"
#include <libconfig.h++>

using libconfig::Config;
using libconfig::Setting;

static const char *BlogUrl = "/";

class BlogApplication : public Wt::WApplication
{
public:
  BlogApplication(const Wt::WEnvironment& env, Wt::Dbo::SqlConnectionPool& blogDb, std::shared_ptr<DbDriverInterface> mainDb)
    : Wt::WApplication(env)
  {
    root()->addWidget(std::make_unique<MainView>(BlogUrl, blogDb, mainDb));
    useStyleSheet("css/style.css");
    useStyleSheet("css/bootstrap.css");
  }
};

std::unique_ptr<Wt::WApplication> createApplication(const Wt::WEnvironment& env,
                                Wt::Dbo::SqlConnectionPool *blogDb, std::shared_ptr<DbDriverInterface> mainDb)
{
  return std::make_unique<BlogApplication>(env, *blogDb, mainDb);
}

int main(int argc, char **argv)
{
  char* argvDebug[] = {"./taskmaster.wt", "--docroot", "../;/css","--approot", "../", "--http-listen", "0.0.0.0", "--resources-dir", "."};
  Config cfg;
  cfg.readFile("config.cfg");
  
  
  try {
    Wt::WServer server(9, argvDebug, WTHTTP_CONFIGURATION);

    Session::configureAuth();

    auto mainDb = std::make_shared<DbDriver>((std::string)cfg.lookup("connection_string"));

    std::unique_ptr<Wt::Dbo::SqlConnectionPool> blogDb
      = Session::createConnectionPool((std::string)cfg.lookup("connection_string"));

    server.addEntryPoint(Wt::EntryPointType::Application,
                         std::bind(&createApplication, std::placeholders::_1, blogDb.get(), mainDb), BlogUrl);

    server.run();

  } catch (Wt::WServer::Exception& e) {
    std::cerr << e.what() << std::endl;
  } catch (std::exception &e) {
    std::cerr << "exception: " << e.what() << std::endl;
  }
}

