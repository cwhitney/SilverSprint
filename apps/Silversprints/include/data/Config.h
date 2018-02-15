/*
 Copyright (c) 2015, Paul Houx - All rights reserved.
 This code is intended for use with the Cinder C++ library: http://libcinder.org
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and
 the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
 the following disclaimer in the documentation and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "cinder/Color.h"
#include "cinder/DataSource.h"
#include "cinder/DataTarget.h"
#include "cinder/Log.h"
#include "cinder/Utilities.h"
#include "cinder/Vector.h"
#include "cinder/app/Event.h"
#include "jsoncpp/json.h"

namespace tools {
    
    using ConfigRef = std::shared_ptr<class Config>;
    
    //! Reads and writes a JSON configuration file. Allows you to specify default values in case a setting does not exist.
    class Config {
    public:
        class Options {
            friend class Config;
            
        public:
            Options()
            : mSetIfDefault( false )
            , mWriteIfSet( false )
            {
            }
            
            //! If \a enabled, add the default value to the configuration file if the key was not found.
            Options &setIfDefault( bool enabled = true )
            {
                mSetIfDefault = enabled;
                return *this;
            }
            //! If \a enabled, write the config file immediately after setting the new value.
            Options &writeIfSet( bool enabled = true )
            {
                mWriteIfSet = enabled;
                return *this;
            }
            
        private:
            bool mSetIfDefault;
            bool mWriteIfSet;
        };
        
    public:
        ~Config() {}
        
        static Config &instance()
        {
            static Config sInstance;
            return sInstance;
        }
        
        void setOptions( const Options &options ) { mOptions = options; }
        
        bool read( const ci::DataSourceRef &source )
        {
            mSource = source;
            
            if( mSource->isFilePath() ) {
                mTarget = ci::DataTargetPath::createRef( mSource->getFilePath() );
            }
            else {
                // We can only write to files on disk.
                mTarget.reset();
            }
            
            CI_LOG_I( "Reading configurations from: " << source->getFilePath() );
            
            try {
                std::string data = ci::loadString( source );
                
                Json::Reader reader;
                reader.parse( data, mRoot );
                
                mDirty = false;
                return true;
            }
            catch( ... ) {
                return false;
            }
        }
        
        bool write() { return write( mTarget ); }
        
        bool write( const ci::DataTargetRef &target )
        {
            if( target ) {
                const ci::fs::path &path = target->getFilePath();
                
                if( !ci::fs::exists( path ) ) {
                    ci::fs::create_directories( path.parent_path() );
                    mDirty = true;
                }
                
                if( mDirty ) {
                    CI_LOG_I( "Writing configurations to: " << target->getFilePath() );
                    
                    Json::StyledWriter writer;
                    std::string        data = writer.write( mRoot );
                    
                    auto stream = target->getStream();
                    if( stream ) {
                        stream->seekAbsolute( 0 );
                        stream->writeData( data.data(), data.size() );
                        mDirty = false;
                    }
                }
                
                return true;
            }
            
            return false;
        }
        
        const ci::fs::path &path() const
        {
            if( mSource && mSource->isFilePath() )
                return mSource->getFilePath();
            
            static ci::fs::path kEmpty;
            return kEmpty;
        }
        
        template <typename T>
        T get( const std::string &category, const std::string &key, const T &defaultValue, const char *comment )
        {
            auto str = mRoot["configuration"][category][key].asString();
            
            if( str.empty() ) {
                if( mOptions.mSetIfDefault ) {
                    set( category, key, defaultValue, comment );
                }
                
                return defaultValue;
            }
            
            try {
                auto result = ci::fromString<T>( str );
                return result;
            }
            catch( const std::exception & ) {
                return defaultValue;
            }
        }
        
        template <typename T>
        T get( const std::string &category, const std::string &key, const T &defaultValue )
        {
            return get<T>( category, key, defaultValue, nullptr );
        }
        
        template <typename T>
        bool set( const std::string &category, const std::string &key, const T &value, const char *comment )
        {
            std::string str;
            if( nullptr != comment ) {
                // Make sure comments start with 2 forward slashes.
                if( strncmp( comment, "//", 2 ) == 0 ) {
                    str.append( comment );
                }
                else {
                    str.append( "// " );
                    str.append( comment );
                }
            }
            else if( mRoot["configuration"].isMember( category ) && mRoot["configuration"][category].isMember( key ) )
                str = mRoot["configuration"][category][key].getComment( Json::CommentPlacement::commentAfterOnSameLine );
            
            mRoot["configuration"][category][key] = Json::Value( value );
            mRoot["configuration"][category][key].setComment( str, Json::CommentPlacement::commentAfterOnSameLine );
            
            mDirty = true;
            
            if( mOptions.mWriteIfSet )
                write();
            
            return true;
        }
        
        template <typename T>
        bool set( const std::string &category, const std::string &key, const T &value )
        {
            return set<T>( category, key, value, nullptr );
        }
        
        static std::string toHex( uint32_t argb )
        {
            std::stringstream stream;
            stream << std::hex << argb;
            return std::string( stream.str() );
        }
        
        static std::string toHex( const ci::ColorA &color )
        {
            uint32_t argb = uint8_t( color.a * 255 ) << 24 | uint8_t( color.r * 255 ) << 16 | uint8_t( color.g * 255 ) << 8 | uint8_t( color.b * 255 );
            return toHex( argb );
        }
        
        static uint32_t fromHex( const std::string &hex )
        {
            std::stringstream stream;
            stream << std::hex << hex;
            
            uint32_t result;
            stream >> result;
            
            return result;
        }
        
    private:
        Config()
        : mDirty( false )
        {
        }
        Config( const Options &options )
        : mDirty( false )
        , mOptions( options )
        {
        }
        
        Config( const Config & ) = delete;
        Config &operator=( const Config & ) = delete;
        
    private:
        Options             mOptions;
        mutable bool        mDirty;
        mutable Json::Value mRoot;
        ci::DataSourceRef   mSource;
        ci::DataTargetRef   mTarget;
    };
    
    template <>
    inline bool Config::set<bool>( const std::string &category, const std::string &key, const bool &value, const char *comment )
    {
        return set( category, key, (int)value );
    }
    
    template <>
    inline bool Config::set<ci::ivec2>( const std::string &category, const std::string &key, const ci::ivec2 &value, const char *comment )
    {
        Json::Value v;
        v["x"] = value.x;
        v["y"] = value.y;
        set( category, key, v, comment );
        return true;
    }
    
    template <>
    inline bool Config::set<ci::ivec3>( const std::string &category, const std::string &key, const ci::ivec3 &value, const char *comment )
    {
        Json::Value v;
        v["x"] = value.x;
        v["y"] = value.y;
        v["z"] = value.z;
        set( category, key, v, comment );
        return true;
    }
    
    template <>
    inline bool Config::set<ci::vec2>( const std::string &category, const std::string &key, const ci::vec2 &value, const char *comment )
    {
        Json::Value v;
        v["x"] = value.x;
        v["y"] = value.y;
        set( category, key, v, comment );
        return true;
    }
    
    template <>
    inline bool Config::set<ci::vec3>( const std::string &category, const std::string &key, const ci::vec3 &value, const char *comment )
    {
        Json::Value v;
        v["x"] = value.x;
        v["y"] = value.y;
        v["z"] = value.z;
        set( category, key, v, comment );
        return true;
    }
    
    template <>
    inline bool Config::set<ci::vec4>( const std::string &category, const std::string &key, const ci::vec4 &value, const char *comment )
    {
        Json::Value v;
        v["x"] = value.x;
        v["y"] = value.y;
        v["z"] = value.z;
        v["w"] = value.w;
        set( category, key, v, comment );
        return true;
    }
    
    template <>
    inline bool Config::set<ci::Color>( const std::string &category, const std::string &key, const ci::Color &value, const char *comment )
    {
        Json::Value v = toHex( value );
        set( category, key, v, comment );
        return true;
    }
    
    template <>
    inline bool Config::set<ci::ColorA>( const std::string &category, const std::string &key, const ci::ColorA &value, const char *comment )
    {
        Json::Value v = toHex( value );
        set( category, key, v, comment );
        return true;
    }
    
    template <>
    inline int Config::get<int>( const std::string &category, const std::string &key, const int &defaultValue, const char *comment )
    {
        auto json = mRoot["configuration"][category][key];
        if( json.empty() ) {
            if( mOptions.mSetIfDefault )
                set( category, key, defaultValue, comment );
            return defaultValue;
        }
        
        return json.asInt();
    }
    
    template <>
    inline float Config::get<float>( const std::string &category, const std::string &key, const float &defaultValue, const char *comment )
    {
        auto json = mRoot["configuration"][category][key];
        if( json.empty() ) {
            if( mOptions.mSetIfDefault )
                set( category, key, defaultValue, comment );
            return defaultValue;
        }
        
        return json.asFloat();
    }
    
    template <>
    inline double Config::get<double>( const std::string &category, const std::string &key, const double &defaultValue, const char *comment )
    {
        auto json = mRoot["configuration"][category][key];
        if( json.empty() ) {
            if( mOptions.mSetIfDefault )
                set( category, key, defaultValue, comment );
            return defaultValue;
        }
        
        return json.asDouble();
    }
    
    template <>
    inline bool Config::get<bool>( const std::string &category, const std::string &key, const bool &defaultValue, const char *comment )
    {
        return get<int>( category, key, (int)defaultValue, comment ) != 0;
    }
    
    template <>
    inline ci::ivec2 Config::get<ci::ivec2>( const std::string &category, const std::string &key, const ci::ivec2 &defaultValue, const char *comment )
    {
        auto json = mRoot["configuration"][category][key];
        if( json.empty() ) {
            if( mOptions.mSetIfDefault )
                set( category, key, defaultValue, comment );
            return defaultValue;
        }
        
        ci::ivec2 v;
        v.x = json["x"].asInt();
        v.y = json["y"].asInt();
        return v;
    }
    
    template <>
    inline ci::ivec3 Config::get<ci::ivec3>( const std::string &category, const std::string &key, const ci::ivec3 &defaultValue, const char *comment )
    {
        auto json = mRoot["configuration"][category][key];
        if( json.empty() ) {
            if( mOptions.mSetIfDefault )
                set( category, key, defaultValue, comment );
            return defaultValue;
        }
        
        ci::ivec3 v;
        v.x = json["x"].asInt();
        v.y = json["y"].asInt();
        v.z = json["z"].asInt();
        return v;
    }
    
    template <>
    inline ci::vec2 Config::get<ci::vec2>( const std::string &category, const std::string &key, const ci::vec2 &defaultValue, const char *comment )
    {
        auto json = mRoot["configuration"][category][key];
        if( json.empty() ) {
            if( mOptions.mSetIfDefault )
                set( category, key, defaultValue, comment );
            return defaultValue;
        }
        
        ci::vec2 v;
        v.x = json["x"].asFloat();
        v.y = json["y"].asFloat();
        return v;
    }
    
    template <>
    inline ci::vec3 Config::get<ci::vec3>( const std::string &category, const std::string &key, const ci::vec3 &defaultValue, const char *comment )
    {
        auto json = mRoot["configuration"][category][key];
        if( json.empty() ) {
            if( mOptions.mSetIfDefault )
                set( category, key, defaultValue, comment );
            return defaultValue;
        }
        
        ci::vec3 v;
        v.x = json["x"].asFloat();
        v.y = json["y"].asFloat();
        v.z = json["z"].asFloat();
        return v;
    }
    
    template <>
    inline ci::vec4 Config::get<ci::vec4>( const std::string &category, const std::string &key, const ci::vec4 &defaultValue, const char *comment )
    {
        auto json = mRoot["configuration"][category][key];
        if( json.empty() ) {
            if( mOptions.mSetIfDefault )
                set( category, key, defaultValue, comment );
            return defaultValue;
        }
        
        ci::vec4 v;
        v.x = json["x"].asFloat();
        v.y = json["y"].asFloat();
        v.z = json["z"].asFloat();
        v.w = json["w"].asFloat();
        return v;
    }
    
    template <>
    inline ci::Color Config::get<ci::Color>( const std::string &category, const std::string &key, const ci::Color &defaultValue, const char *comment )
    {
        auto json = mRoot["configuration"][category][key];
        if( json.empty() ) {
            if( mOptions.mSetIfDefault )
                set( category, key, defaultValue, comment );
            return defaultValue;
        }
        
        return ci::Color::hex( fromHex( json.asString() ) );
    }
    
    template <>
    inline ci::ColorA Config::get<ci::ColorA>( const std::string &category, const std::string &key, const ci::ColorA &defaultValue, const char *comment )
    {
        auto json = mRoot["configuration"][category][key];
        if( json.empty() ) {
            if( mOptions.mSetIfDefault )
                set( category, key, defaultValue, comment );
            return defaultValue;
        }
        
        return ci::Color::hex( fromHex( json.asString() ) );
    }
    
    static inline Config &config()
    {
        return Config::instance();
    }
    
    // ------------------------------------------------------------------------------------------------
    
    class ConfigEvent : public ci::app::Event {
    public:
        ConfigEvent( Config &config )
        : mConfig( config )
        {
        }
        
        Config &getConfig() const { return mConfig; }
        
    private:
        Config &mConfig;
    };
    
    class ConfigChangedEvent : public ConfigEvent {
    public:
        ConfigChangedEvent( Config &config )
        : ConfigEvent( config )
        {
        }
    };
    
} // namespace tools
