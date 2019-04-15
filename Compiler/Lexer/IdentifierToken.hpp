// Copyright (C) 2018-2019 Jonathan Müller <jonathanmueller.dev@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#ifndef FOXLEXER_IDENTIFIER
#define FOXLEXER_IDENTIFIER

#include <LiteralToken.hpp>
#include <RuleToken.hpp>

namespace FoxLexer {

    template <class Derived, class TokenSpec>
    struct identifier_token : rule_token<Derived, TokenSpec> {
        static constexpr const char * name = "<identifier>";
    };

    template <class Token>
    struct is_identifier_token : detail::is_token_impl<identifier_token, Token>::value { };

    template <class Token>
    struct is_non_identifier_rule_token
    : std::integral_constant<bool,
                             is_rule_token<Token>::value && !is_identifier_token<Token>::value> { };

    template <char... Char>
    struct keyword_token : literal_token<Char...> { };

#define FOXLEXER_KEYWORD(String) FOXLEXER_DETAIL_STRING(FoxLexer::keyword_token, String)

    template <class Token>
    struct is_keyword_token : detail::is_literal_token_impl<keyword_token, Token>::value {};

    template <class Token>
    struct is_non_keyword_literal_token
    : std::integral_constant<bool,
                             is_literal_token<Token>::value && !is_keyword_token<Token>::value> {};

}

#endif
