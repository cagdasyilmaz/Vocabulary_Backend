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

#include "MethodHandlers.h"

namespace Vocabulary
{
    using json = nlohmann::json;

    void get_method_handler(const std::shared_ptr<restbed::Session>& session)
    {
        const auto& request = session->get_request();

        if(request->get_method() == "OPTIONS"){
            session->close(restbed::OK);
        }

        VOCABULARY_CLIENT_INFO("A GET message is received for URI {}, username: {}, type: {}.",
                               session->get_request()->get_path(),
                               session->get_request()->get_query_parameter("username"),
                               session->get_request()->get_query_parameter("type"));

        std::string username = session->get_request()->get_query_parameter("username");
        std::string type = session->get_request()->get_query_parameter("type");
        //type = get_uri_without_suffix(type, "_backend");

        if(mapStringToVocabularyType.count(type)) {
            VocabularyType vocabularyType = mapStringToVocabularyType[type];

            std::size_t number_of_words = get_number_of_words(vocabularyType);

            std::vector<int> indexes = VocabularyDatabaseSQLite::getInstance().get_word_indexes(username, type);

            // Using initializer lists
            json size_and_indexes = {
                    {"size", number_of_words},
                    {"OptionIndex_1", indexes[0]},
                    {"OptionIndex_2", indexes[1]},
                    {"OptionIndex_3", indexes[2]},
                    {"OptionIndex_4", indexes[3]},
            };

            std::string j_string = to_string(size_and_indexes);

            session->close(restbed::OK, j_string,
                           {{"Content-Type", "application/json"},
                            {"Content-Length", std::to_string(j_string.size())},
                           });
        }
        else{
            VOCABULARY_CLIENT_ERROR("This vocabulary type is not implemented in backend yet!");
            session->close(restbed::INTERNAL_SERVER_ERROR);
        }
    }

    void post_method_handler(const std::shared_ptr<restbed::Session>& session)
    {
        const auto& request = session->get_request();

        if(request->get_method() == "OPTIONS"){
            session->close(restbed::OK);
        }
        VOCABULARY_CLIENT_INFO("A POST message is received for URI {}, from: {}.",
                               session->get_request()->get_path(),
                               session->get_destination());



        std::size_t length{};
        length = request->get_header("Content-Length", length);
        if (length == 0)
        {
            VOCABULARY_CLIENT_ERROR("Not Acceptable, empty request body!");
            session->close(restbed::LENGTH_REQUIRED);
            return;
        }

        session->fetch(length, [=]
                (const std::shared_ptr<restbed::Session> &session, const restbed::Bytes &data) {

            std::string username;
            std::string type;
            std::string option;
            std::size_t index{0};

            std::string j_string;

            const std::string str{reinterpret_cast<const char *>(data.data()), data.size()};

            json post_data = json::parse(str);
            username = post_data.at("username").get<std::string>();
            type = post_data.at("type").get<std::string>();
            option = post_data.at("mode").get<std::string>();
            index = post_data.at("index").get<std::size_t>();

            VocabularyType vocabularyType = mapStringToVocabularyType[type];

            VocabularyDatabaseSQLite::getInstance().update(username, type, option, index);

            if (option == "2" || option == "4") {
                int index_unordered = ThreadSafe_JSON_Users::getInstance().check_and_read(username, type, option, index, get_number_of_words(vocabularyType));

                if (index_unordered == -1) {
                    ThreadSafe_JSON_Users::getInstance().write_file();
                    index_unordered = ThreadSafe_JSON_Users::getInstance().read(username, type, option, index);
                }
                index = index_unordered;
            }

            //VOCABULARY_CORE_INFO("before j_string: {}", str);

            j_string = get_word_as_json(vocabularyType, index);

            if (j_string.empty()) {
                VOCABULARY_CLIENT_ERROR("This vocabulary type is not implemented in backend yet!");
                session->close(restbed::INTERNAL_SERVER_ERROR);
                return;
            }

            //VOCABULARY_CORE_INFO("Before close!");
            //VOCABULARY_CORE_INFO("j_string: {}", j_string);

            session->close(restbed::OK, j_string,
                           {{"Content-Type",   "application/json"},
                            {"Content-Length", std::to_string(j_string.size())},
                           });
            //promise.set_value();
            //VOCABULARY_CORE_INFO("After close!");

        });

        //future.wait(); // Wait for the completion of the fetch operation
        //j_string.clear();
    }

    void options_method_handler(const std::shared_ptr<restbed::Session>& session)
    {
        const auto& request = session->get_request();
        VOCABULARY_CLIENT_INFO("A OPTIONS message is received for URI {}, from: {}.",
                               session->get_request()->get_path(),
                               session->get_destination());

        session->close(restbed::OK);
    }

    /// Get number of Words
    std::size_t get_number_of_words(VocabularyType vocabularyType)
    {
        switch (vocabularyType)
        {
            case VocabularyType::Verbs:
                return ThreadSafe_JSON_Verbs::getInstance().size();

            case VocabularyType::Advanced_Verbs:
                return ThreadSafe_JSON_Advanced_Verbs::getInstance().size();

            case VocabularyType::Phrasal_Verbs:
                return ThreadSafe_JSON_Phrasal_Verbs::getInstance().size();

            case VocabularyType::Nouns:
                return ThreadSafe_JSON_Nouns::getInstance().size();

            case VocabularyType::Advanced_Nouns:
                return ThreadSafe_JSON_Advanced_Nouns::getInstance().size();

            case VocabularyType::Adjectives:
                return ThreadSafe_JSON_Adjectives::getInstance().size();

            case VocabularyType::Advanced_Adjectives:
                return ThreadSafe_JSON_Advanced_Adjectives::getInstance().size();

            case VocabularyType::Adverbs:
                return ThreadSafe_JSON_Adverbs::getInstance().size();

            case VocabularyType::Advanced_Adverbs:
                return ThreadSafe_JSON_Advanced_Adverbs::getInstance().size();

            case VocabularyType::Prepositions:
                return ThreadSafe_JSON_Prepositions::getInstance().size();

            case VocabularyType::Advanced_Prepositions:
                return ThreadSafe_JSON_Advanced_Prepositions::getInstance().size();

            default:
                return 0;
        }
    }

    /// Get word information as a json
    std::string get_word_as_json(VocabularyType vocabularyType, size_t index)
    {
        std::string j_string;
        switch (vocabularyType)
        {
            case VocabularyType::Verbs:
                j_string = to_string(ThreadSafe_JSON_Verbs::getInstance().read(index));
                break;

            case VocabularyType::Advanced_Verbs:
                j_string = to_string(ThreadSafe_JSON_Advanced_Verbs::getInstance().read(index));
                break;

            case VocabularyType::Phrasal_Verbs:
                j_string = to_string(ThreadSafe_JSON_Phrasal_Verbs::getInstance().read(index));
                break;

            case VocabularyType::Nouns:
                j_string = to_string(ThreadSafe_JSON_Nouns::getInstance().read(index));
                break;

            case VocabularyType::Advanced_Nouns:
                j_string = to_string(ThreadSafe_JSON_Advanced_Nouns::getInstance().read(index));
                break;

            case VocabularyType::Adjectives:
                j_string = to_string(ThreadSafe_JSON_Adjectives::getInstance().read(index));
                break;

            case VocabularyType::Advanced_Adjectives:
                j_string = to_string(ThreadSafe_JSON_Advanced_Adjectives::getInstance().read(index));
                break;

            case VocabularyType::Adverbs:
                j_string = to_string(ThreadSafe_JSON_Adverbs::getInstance().read(index));
                break;

            case VocabularyType::Advanced_Adverbs:
                j_string = to_string(ThreadSafe_JSON_Advanced_Adverbs::getInstance().read(index));
                break;

            case VocabularyType::Prepositions:
                j_string = to_string(ThreadSafe_JSON_Prepositions::getInstance().read(index));
                break;

            case VocabularyType::Advanced_Prepositions:
                j_string = to_string(ThreadSafe_JSON_Advanced_Prepositions::getInstance().read(index));
                break;

            default:
                j_string.clear();
        }

        return j_string;
    }

    std::string get_uri_without_suffix(const std::string& str, const std::string& suffix) {
        // Find the position of the suffix starting from the end.
        size_t pos = str.rfind(suffix);
        if (pos == std::string::npos) {
            // Suffix not found, return the entire string.
            return str;
        }

        // If the suffix is at the end, return the substring before it.
        if (pos == str.length() - suffix.length()) {
            return str.substr(0, pos);
        } else {
            return str;
        }
    }

}
