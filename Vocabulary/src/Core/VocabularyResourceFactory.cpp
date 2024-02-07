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

#include "VocabularyResourceFactory.h"

namespace Vocabulary
{
    VocabularyResourceFactory::VocabularyResourceFactory()
    {
        resource_ = std::make_shared<restbed::Resource>();
        setPaths();
        resource_->set_paths(paths);
        resource_->set_method_handler("GET", get_method_handler);
        resource_->set_method_handler("POST", post_method_handler);
        resource_->set_method_handler("OPTIONS", options_method_handler);

        resource_->set_default_header("Access-Control-Allow-Origin", "*");
        resource_->set_default_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        resource_->set_default_header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept, *");
        resource_->set_default_header("Accept", "*/*");
        resource_->set_default_header("Access-Control-Allow-Private-Network", "true");
        resource_->set_default_header("Access-Control-Allow-Credentials","true");
    }

    std::shared_ptr<restbed::Resource> VocabularyResourceFactory::get_resource() const
    {
        return resource_;
    }

    void VocabularyResourceFactory::setPaths()
    {
        paths = {"/home", "/login", "/verbs", "/advanced_verbs", "/phrasal_verbs", "/nouns", "/advanced_nouns",
                 "/adjectives", "/advanced_adjectives", "/adverbs", "/advanced_adverbs", "/prepositions",
                 "/advanced_prepositions", "/copyright"};
    }
}