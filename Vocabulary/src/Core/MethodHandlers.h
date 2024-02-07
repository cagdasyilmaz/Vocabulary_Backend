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

#pragma once

#include <restbed>
#include <nlohmann/json.hpp>

#include "Logger.h"
#include "../../../Vocabulary/json_singletons/JSONDatabaseVocabulary.h"
#include "../../../Vocabulary/json_singletons/JSONDatabaseUser.h"
#include "../../../Vocabulary/sqlite_database/VocabularyDatabaseSQLite.h"

#include <string>
#include <map>
#include <vector>
#include <cstddef>

namespace Vocabulary {

    enum class VocabularyType
    {
        Verbs,
        Advanced_Verbs,
        Phrasal_Verbs,
        Nouns,
        Advanced_Nouns,
        Adjectives,
        Advanced_Adjectives,
        Adverbs,
        Advanced_Adverbs,
        Prepositions,
        Advanced_Prepositions
    };

    static std::map<std::string, VocabularyType> mapStringToVocabularyType{
            {"verbs", VocabularyType::Verbs},
            {"advanced_verbs", VocabularyType::Advanced_Verbs},
            {"phrasal_verbs", VocabularyType::Phrasal_Verbs},
            {"nouns", VocabularyType::Nouns},
            {"advanced_nouns", VocabularyType::Advanced_Nouns},
            {"adjectives", VocabularyType::Adjectives},
            {"advanced_adjectives", VocabularyType::Advanced_Adjectives},
            {"adverbs", VocabularyType::Adverbs},
            {"advanced_adverbs", VocabularyType::Advanced_Adverbs},
            {"prepositions", VocabularyType::Prepositions},
            {"advanced_prepositions", VocabularyType::Advanced_Prepositions},
    };

    /// Method handler function for "GET" request in HTPP messages
    void get_method_handler(const std::shared_ptr<restbed::Session>&);

    /// Method handler function for "GET" request in HTPP messages
    void post_method_handler(const std::shared_ptr<restbed::Session>&);

    /// Method handler function for "OPTIONS" request in HTPP messages
    void options_method_handler(const std::shared_ptr<restbed::Session>&);

    /**
     * Helper functions for method handlers
     */

    /// Get number of Words
    std::size_t get_number_of_words(VocabularyType vocabularyType);

    /// Get word information as a json
    std::string get_word_as_json(VocabularyType, size_t);

    /// Get URI name without suffix for backend operations
    std::string get_uri_without_suffix(const std::string&, const std::string&);
}






