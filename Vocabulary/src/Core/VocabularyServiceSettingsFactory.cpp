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

#include "VocabularyServiceSettingsFactory.h"

namespace Vocabulary
{
    VocabularyServiceSettingsFactory::VocabularyServiceSettingsFactory()
    {
        settings_ = std::make_shared<restbed::Settings>();

        //auto ssl_settings = std::make_shared<restbed::SSLSettings>();
        //ssl_settings->set_http_disabled(true);

        //ssl_settings->set_private_key( restbed::Uri("file:///etc/letsencrypt/live/ismailcagdasyilmaz.com/privkey.pem"));

        //ssl_settings->set_certificate( restbed::Uri("file:///etc/letsencrypt/live/ismailcagdasyilmaz.com/fullchain.pem"));

        //ssl_settings->set_temporary_diffie_hellman(restbed::Uri("file:///home/cagdasyilmazhb/CLionProjects/Vocabulary_Backend/build/dh2048.pem"));
        //settings_->set_ssl_settings(ssl_settings);

        settings_->set_port(5173);
        settings_->set_worker_limit(std::thread::hardware_concurrency());
        settings_->set_default_header("Connection", "keep-alive");
    }

    std::shared_ptr<restbed::Settings> VocabularyServiceSettingsFactory::get_settings() const
    {
        return settings_;
    }
}
