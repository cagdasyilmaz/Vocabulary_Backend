/****************************************************************************
 * MIT License
 *
 * Copyright (c) 2024 İsmail Çağdaş Yılmaz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ****************************************************************************/

#include "../../../Vocabulary/include/Core/RestbedAPILogger.h"

namespace Vocabulary {
    void SyslogLogger::stop( void )
    {
        return;
    }

    void SyslogLogger::start(const std::shared_ptr<const restbed::Settings> &)
    {
        return;
    }

    void SyslogLogger::log(const Level level, const char* format, ... )
    {
        setlogmask( LOG_UPTO( LOG_DEBUG ) );

        openlog( "Vocabulary_Backend", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1 );

        int priority = 0;

        switch ( level )
        {
            case FATAL:
                priority = LOG_CRIT;
                break;

            case ERROR:
                priority = LOG_ERR;
                break;

            case WARNING:
                priority = LOG_WARNING;
                break;

            case SECURITY:
                priority = LOG_ALERT;
                break;

            case INFO:
            case DEBUG:
            default:
                priority = LOG_NOTICE;
        }

        va_list arguments;

        va_start( arguments, format );

        vsyslog( priority, format, arguments );

        va_end( arguments );

        closelog( );
    }

    void SyslogLogger::log_if( bool expression, const Level level, const char* format, ... )
    {
        if ( expression )
        {
            va_list arguments;

            va_start( arguments, format );

            log( level, format, arguments );

            va_end( arguments );
        }
    }
}