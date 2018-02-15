#pragma once

using namespace ci;
using namespace std;

using CsvLoggerRef = std::shared_ptr<class CsvLogger>;

class CsvLogger
{
  public:
    // ------------------------------------------------------------
    class CsvOptions
    {
        friend class CsvLogger;
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
        RACE_START,
        RACE_STOP,
        RACE_FINISH,
        RACE_FALSE_START
    } SS_EVENT;
    
    std::string enumToStr(SS_EVENT evt){
        switch (evt) {
            case SS_EVENT::RACE_START:          return "RACE_START";
            case SS_EVENT::RACE_STOP:           return "RACE_STOP";
            case SS_EVENT::RACE_FINISH:         return "RACE_FINISH";
            case SS_EVENT::RACE_FALSE_START:    return "RACE_FALSE_START";
            default:
                break;
        }
        
        return "";
    }
    
    static CsvLogger &instance()
    {
        static CsvLogger sInstance;
        return sInstance;
    }
    
    CsvLogger(const CsvLogger &) = delete;
    CsvLogger &operator=(const CsvLogger &) = delete;
    
    void setHeaders (const std::vector<std::string> &headerList)
    {
        mHeaderList = headerList;
    }
    
    //! Timestamp will log in ISO 8601 format: 2018-01-24T18:19:57
    template< typename ... Args >
    void log(const SS_EVENT &eventType, Args const& ... args) {
        
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
        strftime(buffer,sizeof(buffer),"%Y_%m_%d_SilverSprintsLog.csv", timeinfo);
        
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
    ~CsvLogger() {};
    CsvLogger() {};
    CsvLogger(CsvOptions opts) : mOptions{opts}
    {}
    
    CsvOptions                  mOptions;
    std::string                 logStr = "";
    std::vector<std::string>    mHeaderList;
};

static inline CsvLogger &raceLog()
{
    return CsvLogger::instance();
}
