/****************************************************************************
 * MIT License
 *
 * Copyright (c) 2019 İsmail Çağdaş Yılmaz
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

#include "Base.h"

using namespace Vocabulary;

int main(int argc, char* argv[])
{
    /// Initialize the Logger for the command-line and the filesystem
    Vocabulary::Logger::init();

    /// Initialize SQLite Database For User Operatation
    initialize_SQLite_Database();

    /// Initialize JSON Databases for Vocabulary Classes and
    initialize_JSON_Vocabulary_Classes();
    initialize_JSON_User_Class();

    auto vocabulary_resource_factory = std::make_shared<VocabularyResourceFactory>();
    auto vocabulary_service_settings_factory = std::make_shared<VocabularyServiceSettingsFactory>();
    VocabularyService vocabulary_service {vocabulary_resource_factory, vocabulary_service_settings_factory};

    try {
        //vocabulary_service.start();
        VOCABULARY_CORE_INFO("WEB service initialized.");
        vocabulary_service.start();
    } catch(const std::exception& e)
    {
        VOCABULARY_CORE_ERROR("WEB service not initialized.");
        VOCABULARY_CORE_ERROR(e.what());
    }

    VOCABULARY_CORE_INFO("The program stopped.");
    return 0;
}



