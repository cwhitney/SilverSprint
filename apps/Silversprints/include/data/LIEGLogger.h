#pragma once

#include "cinder/app/App.h"

using namespace ci;
using namespace std;

namespace hush {
    
    using LIEGLoggerRef = std::shared_ptr<class LIEGLogger>;
    
    class LIEGLogger {
    public:
        // ------------------------------------------------------------
        class CsvOptions
        {
            friend class LIEGLogger;
        public:
            CsvOptions() : bAutoTimestamp(true)
            {
            };
            
            CsvOptions autoTimestamp(bool autoTimestamp){
                bAutoTimestamp = autoTimestamp;
                return *this;
            }
        protected:
            bool bAutoTimestamp = true;
        };
        
        // ------------------------------------------------------------
        typedef enum {
            WAKE,
            OUR_WORLD,
            OUR_HEROS,
            HERO_SELECT_1,
            HERO_SELECT_2,
            HERO_SELECT_3,
            HERO_SELECT_4,
            HERO_SELECT_5,
            HERO_SELECT_6,
            HOME
        } LIEG_EVENT;
        
        std::string enumToStr(LIEG_EVENT evt){
            switch (evt) {
                case LIEG_EVENT::WAKE:          return "WAKE";
                case LIEG_EVENT::OUR_WORLD:     return "OUR_WORLD";
                case LIEG_EVENT::OUR_HEROS:     return "OUR_HEROS";
                case LIEG_EVENT::HERO_SELECT_1: return "HERO_SELECT_1";
                case LIEG_EVENT::HERO_SELECT_2: return "HERO_SELECT_2";
                case LIEG_EVENT::HERO_SELECT_3: return "HERO_SELECT_3";
                case LIEG_EVENT::HERO_SELECT_4: return "HERO_SELECT_4";
                case LIEG_EVENT::HERO_SELECT_5: return "HERO_SELECT_5";
                case LIEG_EVENT::HERO_SELECT_6: return "HERO_SELECT_6";
                case LIEG_EVENT::HOME:          return "HOME";
                default:
                    break;
            }
            
            return "";
        }
        
        static LIEGLogger &instance() {
            static LIEGLogger mInstance;
            return mInstance;
        }
        
        LIEGLogger(const LIEGLogger &) = delete;
        LIEGLogger &operator=(const LIEGLogger &) = delete;
        
        //! Timestamp will log in ISO 8601 format: 2018-01-24T18:19:57
        template< typename ... Args >
        void log(const LIEG_EVENT &eventType, Args const& ... args) {
            
            if(mOptions.bAutoTimestamp){
                time_t now;
                time(&now);
                char buf[sizeof "2011-10-08T07:07:09Z"];
                strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));
                logStr.append(buf);
                logStr.append(",");
            }
            
            logStr.append(enumToStr(eventType));
            
            // Parge the rest of the args
            // using pack expansion inside a braced-init-list
            std::ostringstream stream;
            using List= int[];
            (void)List{0, ( (void)(stream << "," << args), 0 ) ... };
            logStr.append( stream.str() );
            logStr.append( "\n" );
        }
        
        void write()
        {
            auto pth = ci::app::getAppPath() /= ci::fs::path("logs");
            if(! fs::exists(pth) ){
                fs::create_directory(pth);
            }
            
            // make a date string
            time_t rawtime;
            struct tm* timeinfo;
            char buffer[80];
            time (&rawtime);
            timeinfo = localtime(&rawtime);
            strftime(buffer,sizeof(buffer),"%Y_%m_%d_LIEGLog.csv", timeinfo);
            
            pth /= fs::path(string(buffer));
            write(pth);
        }
        
        void write(ci::fs::path pth)
        {
            
            if(fs::exists(pth)){
                std::ifstream t(pth.string());
                std::stringstream buffer;
                buffer << t.rdbuf();
                t.close();
                
                std::ofstream outfile(pth.string());
                outfile << buffer.str() << logStr;
                outfile.close();
            }else{
                std::ofstream outfile(pth.string());
                outfile << logStr;
                outfile.close();
            }
        }
        
        void clear()
        {
            logStr = "";
        }
        
    protected:
        LIEGLogger() {};
        LIEGLogger(CsvOptions opts) : mOptions{opts}
        {}
        CsvOptions      mOptions;
        std::string     logStr = "";
    };
    
    static inline LIEGLogger &liegLog()
    {
        return LIEGLogger::instance();
    }
}


