#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "args_gets.hpp"
#include "csrc.hpp"
#include "../clib/str.hpp"
#include "../clib/err.hpp"
#include "../clib/files.hpp"
#include "../clib/vectors.hpp"
#include "../clib/args.hpp"

using namespace std;
using namespace clib;


#define R_BOUNDARY "\\b"
#define R_CH_WHITESPACE "\\s"
#define R_CH_SYMBOL_HEAD "[a-zA-Z_]"
#define R_CH_SYMBOL_BODY "[a-zA-Z0-9_]"

#define R_SYMBOL \
    R_BOUNDARY \
    R_CH_SYMBOL_HEAD \
    R_CH_SYMBOL_BODY "*" \
    R_BOUNDARY


#define R_CH_NOT_BLOCK_OPEN "[^\\{]"
#define R_CH_BLOCK_OPEN "\\{"


#define R_BLOCK_OPEN \
    R_CH_NOT_BLOCK_OPEN "*" \
    R_CH_BLOCK_OPEN

#define R_NAMESPACE_SEP "::"

#define R_NAMESPACE_SYMBOL \
    R_BOUNDARY \
    "("  \
        R_CH_WHITESPACE "*" \
        R_NAMESPACE_SEP \
        R_CH_WHITESPACE "*" \
        "|"  \
        R_CH_SYMBOL_HEAD \
        R_CH_SYMBOL_BODY "*" \
    ")*" \
    R_BOUNDARY

#define R_NAMESPACE_BLOCK_OPEN \
    R_CH_WHITESPACE "*" \
    R_CH_BLOCK_OPEN

// namespace regex
#define R_NAMESPACE \
    R_BOUNDARY \
    "namespace" \
    R_CH_WHITESPACE "+" \
    R_NAMESPACE_SYMBOL \
    R_NAMESPACE_BLOCK_OPEN "$"

#define R_TEMPLATE_ARGS "<[^>]*>"

#define R_TEMPLATE \
    R_BOUNDARY \
    "(template)" \
    R_BOUNDARY \
    R_CH_WHITESPACE "*" \
    "(" R_TEMPLATE_ARGS ")" \
    R_CH_WHITESPACE "*"

// class or struct regex
#define R_CLASS_STRUCT \
    "(" R_TEMPLATE "){0,1}" \
    R_BOUNDARY \
    "(class|struct)" \
    R_BOUNDARY \
    R_CH_WHITESPACE "+" \
    "(" R_SYMBOL ")" \
    R_BLOCK_OPEN "$"

#define R_CH_OP_REF "\\&"
#define R_CH_OP_PTR "\\*"
#define R_CH_OP_DESTRUCTOR "\\~"

#define R_CH_FUNCTION_PRE_OP \
    "[" \
        R_CH_OP_REF \
        R_CH_OP_PTR \
        R_CH_OP_DESTRUCTOR \
    "]"

#define R_CH_PARENTHESIS_OPEN "\\("
#define R_CH_PARENTHESIS_CLOSE "\\)"
#define R_CH_NOT_PARENTHESIS_CLOSE "[^\\)]"

#define R_FUNCTION_ARGS \
    R_CH_PARENTHESIS_OPEN \
    "(" R_CH_NOT_PARENTHESIS_CLOSE "*)" \
    R_CH_PARENTHESIS_CLOSE

// function regex
#define R_FUNCTION \
    "(" R_TEMPLATE "){0,1}" \
    "(" R_SYMBOL "|" R_CH_FUNCTION_PRE_OP ")*" \
    "(" R_SYMBOL ")" \
    R_CH_WHITESPACE "*" \
    R_FUNCTION_ARGS \
    "(" R_BLOCK_OPEN ")" "$"


#define R_NO_BLOCK_OPEN "[^{]*;"

#define R_UNDEF_VIRTUAL_FUNCTION \
    "(" R_SYMBOL "|" R_CH_FUNCTION_PRE_OP ")*" \
    R_BOUNDARY \
    "(virtual)" \
    R_BOUNDARY \
    R_CH_WHITESPACE "*" \
    "(" \
        "(" R_SYMBOL "|" R_CH_FUNCTION_PRE_OP ")*" \
        R_CH_WHITESPACE "*" \
    ")+" \
    R_FUNCTION_ARGS \
    "(" R_NO_BLOCK_OPEN ")" "$"


const string r_namespace = R_NAMESPACE;
const string r_class_struct = R_CLASS_STRUCT;
const string r_function = R_FUNCTION;
const string r_undef_virtual_function = R_UNDEF_VIRTUAL_FUNCTION;

enum block_type { NAMESPACE, CLASS_STRUCT, FUNCTION };

struct block_level {
    int level;
    block_type type;
    string head;
    vector<string> matches;
    bool is_template;
};

bool is_tail_at_namespace(
    const string& tail, 
    const vector<block_level>& levels, 
    vector<string>& matches
) {
    for (const block_level& level: levels)
        if (level.type == CLASS_STRUCT || level.type == FUNCTION) return false;
    return regx_match(r_namespace, tail, &matches);
}

bool is_tail_at_class_or_struct(
    const string& tail, 
    const vector<block_level>& levels, 
    vector<string>& matches
) {
    for (const block_level& level: levels)
        if (level.type == FUNCTION) return false;
    return regx_match(r_class_struct, tail, &matches);
}

bool parse_tail_function(
    const string& tail, 
    vector<string>& matches
) {
    return regx_match(r_function, tail, &matches); 
}

bool is_tail_at_function(
    const string& tail, 
    const vector<block_level>& levels, 
    vector<string>& matches
) {
    for (const block_level& level: levels)
        if (level.type == FUNCTION) return false;
    return parse_tail_function(tail, matches);
}

bool is_tail_at_undef_virtual_function(
    const string& tail, 
    const vector<block_level>& levels, 
    vector<string>& matches
) {
    for (const block_level& level: levels)
        if (level.type == FUNCTION) return false;
    return regx_match(r_undef_virtual_function, tail, &matches); 
}

bool is_tail_at_constructor(
    const string& tail, 
    const vector<block_level>& levels, 
    const vector<string>& matches
) {
    string clazz = "";
    for (const block_level& level: levels)
        if (level.type == CLASS_STRUCT)
            clazz = level.matches[5];
    return !clazz.empty() && matches[5] == clazz;
}

#define R_

bool is_tail_at_destructor(
    const string& tail, 
    const vector<block_level>& levels, 
    const vector<string>& matches
) {
    return is_tail_at_constructor(tail, levels, matches) && matches[4] == "~";
}

#define R_FUNCTION_SPEC \
    "(.*)" \
    "(" R_SYMBOL ")" \
    R_CH_WHITESPACE "*" \
    R_FUNCTION_ARGS \
    "(" R_BLOCK_OPEN ")" "$"

#define R_FUNCTION_TPL \
    "(" R_TEMPLATE ")" \
    "(.*)" \
    "(" R_SYMBOL ")" \
    R_CH_WHITESPACE "*" \
    R_FUNCTION_ARGS \
    "(" R_BLOCK_OPEN ")" "$"

const string r_function_spec = R_FUNCTION_SPEC;
const string r_function_tpl = R_FUNCTION_TPL;


vector<string> get_function_spec(const string& tail) {
    vector<string> matches;
    if (!regx_match(r_function_spec, tail, &matches) || matches.size() < 2)
        throw ERROR("Unable to recognize syntax: " + tail);
    return matches;
}

string remove_func_spec_decl_only_keywords(const string& spec) {
    string clean = spec;
    clean = regx_replace_all("\\bvirtual\\b", clean, "");
    clean = regx_replace_all("\\bstatic\\b", clean, "");
    // Note: add more keyword if necessary...
    return clean;  
}

string remove_func_qualifier_decl_only_keywords(const string& spec) {
    string clean = spec;
    clean = regx_replace_all("\\boverride\\b", clean, "");
    // Note: add more keyword if necessary...
    return clean;
}

class SourceCodeTailer {}; // TODO

string remove_func_args_defvals(const string& args) {
    string clean = "";
    string tail = "";
    bool is_defval = false;
    bool inline_comment = false;
    bool multiline_comment = false;
    bool in_string = false; 
    bool in_R_string = false;
    int deep = 0;
    for (char c: args) {
        tail += c;

        if (!inline_comment && str_ends_with("//", tail)) {
            inline_comment = true;
            continue;
        }

        if (inline_comment && str_ends_with("\n", tail)) {
            inline_comment = false;
            continue;
        }

        if (inline_comment) continue;

        if (!multiline_comment && str_ends_with("/*", tail)) {
            multiline_comment = true;
            continue;
        }

        if (multiline_comment && str_ends_with("*/", tail)) {
            multiline_comment = false;
            continue;
        }

        if (multiline_comment) continue;

        if (!in_R_string && str_ends_with("R\"(", tail)) {
            in_R_string = true;
            in_string = false;
            continue;
        }

        if (in_R_string && str_ends_with(")\"", tail)) {
            in_R_string = false;
            continue;
        }

        if (in_R_string) continue;
        
        if (!in_string && c == '"') {
            in_string = true;
            continue;
        }
        
        if (in_string && c == '"') {
            vector<string> matches;
            if (!regx_match("\\+\\\"$", tail, &matches) || matches[0].length() % 2 == 0) // !esc
                in_string = false;
            continue;
        }

        if (in_string) continue;

        if (c == '(') {
            deep++;
            continue;
        }
        if (c == ')') {
            deep--;
            if (deep == 0) break;
            continue;
        }

        if (deep == 0) continue;

        if (!is_defval && c == '=') {
            is_defval = true;
            continue;
        }
        if (is_defval && c == ',') {
            is_defval = false;
            clean += c;
            continue;
        }
        if (is_defval) continue;

        clean += c;
    }
    return clean;
}

vector<string> get_function_template(const string& tail) {
    vector<string> matches;
    if (!regx_match(r_function_tpl, tail, &matches) || matches.size() < 2)
        return {};
    return matches;
}

struct FileContent {
    string filename;
    string content;
    bool modified = true;
    void save() const {
        if (!modified) return;
        const string filepath = path_extract(filename);
        if (!file_exists(filepath)) file_create_path(filepath);
        file_put_contents(filename, content);
    }
};

bool is_tail_at_directive(const string& tail) {
    return regx_match("\\n\\s*\\#$", tail) || regx_match("^\\s*\\#$", tail);
}

void hppcut_file(
    vector<FileContent>& fileContents,
    const string& hppFile, 
    const string& hppcutOutputFolder, 
    const string& hExtension, 
    const string& cppExtension
) {
    // cout << "hppcut_file:" << endl
    //     << "hppFile: " << hppFile << endl
    //     << "hppcutOutputFolder: " << hppcutOutputFolder << endl
    //     << "hExtension: " << hExtension << endl
    //     << "cppExtension: " << cppExtension << endl;

    const string hFilename = file_replace_extension(hppFile, hExtension);
    const string cppFilename = file_replace_extension(hppFile, cppExtension);
    const string hFile = path_normalize(hppcutOutputFolder + "/" + hFilename);
    const string cppFile = path_normalize(hppcutOutputFolder + "/" + cppFilename);

    // cout << "Parsing: " COLOR_FILENAME << hppFile << COLOR_DEFAULT;
    if (file_exists(hFile) && file_get_mtime(hFile) >= file_get_mtime(hppFile) &&
        file_exists(cppFile) && file_get_mtime(cppFile) >= file_get_mtime(hppFile)) {
            fileContents.push_back({ hFile, "", false });
            fileContents.push_back({ cppFile, "", false });
            // cout << " (skip)" << endl;
            return; // no updates - skipp
        }

    cout << "Parsing: " COLOR_FILENAME << hppFile << COLOR_DEFAULT << endl;

    const string hppSource = file_get_contents(hppFile);
    
    bool directive = false;
    bool inline_comment = false;
    bool multiline_comment = false;
    bool in_string = false; 
    bool in_R_string = false;
    int block = 0;
    vector<block_level> levels;
    string tail = "";
    string intr = ""; // .hpp files should have include guard but if not we can make sure to add it here...
    string impl = "#include \"" + filename_extract(hFile) + "\"\n";
    for (const char& c: hppSource) {
        tail += c;

        if (!directive && is_tail_at_directive(tail)) {
            directive = true;
            continue;
        }

        if (directive && str_ends_with("\n", tail) && !str_ends_with("\\\n", tail)) {
            directive = false;
            intr += tail;
            tail = "\n";
            continue;
        }

        if (directive) continue;

        if (!inline_comment && str_ends_with("//", tail)) {
            inline_comment = true;
            continue;
        }

        if (inline_comment && str_ends_with("\n", tail)) {
            inline_comment = false;
            continue;
        }

        if (inline_comment) continue;

        if (!multiline_comment && str_ends_with("/*", tail)) {
            multiline_comment = true;
            continue;
        }

        if (multiline_comment && str_ends_with("*/", tail)) {
            multiline_comment = false;
            continue;
        }

        if (multiline_comment) continue;

        if (!in_R_string && str_ends_with("R\"(", tail)) {
            in_R_string = true;
            in_string = false;
            continue;
        }

        if (in_R_string && str_ends_with(")\"", tail)) {
            in_R_string = false;
            continue;
        }

        if (in_R_string) continue;
        
        if (!in_string && c == '"') {
            in_string = true;
            continue;
        }
        
        if (in_string && c == '"') {
            vector<string> matches;
            if (!regx_match("\\+\\\"$", tail, &matches) || matches[0].length() % 2 == 0) // !esc
                in_string = false;
            continue;
        }

        if (in_string) continue;

        if (c == ';') {
            vector<string> matches;
            if (is_tail_at_undef_virtual_function(tail, levels, matches)) {

                intr += tail; //str_rtrim(tail.substr(0, tail.size() - 1)) + ";";
                tail = ""; 

                // vector<string> classes;
                // for (const block_level& level: levels) 
                //     if (level.type == CLASS_STRUCT) 
                //         classes.push_back(level.matches[5]);
                // const string function_name = 
                //     (classes.empty() ? "" : (vector_concat(classes, "::") + "::")) + matches[4];
                // impl += "\n" + function_name + "(" + 
                //         remove_func_args_defvals(matches[0]) + 
                //     ") { std::runtime_error(\"Unimplemented: " + function_name + "\"); }";

                continue;
            }
            // if (is_tail_at_function(tail, levels, matches)) {
            //     impl += tail; //str_rtrim(tail.substr(0, tail.size() - 1)) + ";";
            // } else {
            //     intr += tail;
            // }
            // tail = ""; 
            // continue;
        }

        if (c == '{') {
            block++;
            // cout << tail << endl;

            vector<string> matches;

            if (is_tail_at_namespace(tail, levels, matches)) {
            // if (regx_match(r_namespace, tail, &matches)) {
                levels.push_back({ block, NAMESPACE, tail, matches });  
                intr += tail;  
                impl += "\n" + matches[0];
                tail = ""; 
                continue;
            }

            if (is_tail_at_class_or_struct(tail, levels, matches)) {
                levels.push_back({ block, CLASS_STRUCT, tail, matches });
                intr += tail;  
                tail = "";  
                continue;
            }

            if (is_tail_at_function(tail, levels, matches)) {
                vector<string> tpl = get_function_template(tail);
                levels.push_back({ block, FUNCTION, tail, matches, !tpl.empty() });

                if (is_tail_at_constructor(tail, levels, matches)) {
                    intr += tpl.empty() ? str_replace(tail, matches[7], ";") : tail;

                    vector<string> classes;
                    for (const block_level& level: levels) 
                        if (level.type == CLASS_STRUCT) 
                            classes.push_back(level.matches[5]); 
                    string cleansrc = tpl.empty() ? "\n" + 
                        (classes.empty() ? "" : (vector_concat(classes, "::") + "::")) + 
                        (is_tail_at_destructor(tail, levels, matches) ? "~" : "") +
                        matches[5] + "(" + 
                            remove_func_args_defvals(matches[0]) + 
                        ")" + matches[7] : "";
                    impl += cleansrc;
                }
                // else if (is_tail_at_destructor(tail, levels, matches)) {
                //     // TODO..
                //     cout << "destr1" << endl;
                // }
                else {
                    // if (hFilename == "clib/Printer.h") {
                    //     if (is_tail_at_destructor(tail, levels, matches)) {
                    //         // TODO..
                    //         cout << "destr2" << endl;
                    //     }
                    // }
                
                    
                    intr += tpl.empty() ? str_rtrim(tail.substr(0, tail.size() - 1)) + ";" : tail;

                    while (true) {
                        vector<string> splits = str_split(";", tail);
                        if (splits.size() <= 1) break;
                        tail = splits[splits.size() - 1];
                    }
                    if (!parse_tail_function(tail, matches))
                        throw ERROR("Parse error");

                    vector<string> classes;
                    for (const block_level& level: levels) 
                        if (level.type == CLASS_STRUCT) 
                            classes.push_back(level.matches[5]); 

                    vector<string> spec = get_function_spec(tail);
                    string till_funcname_starts = spec[1];
                    string funcname = matches[5];
                    string funcargs = matches[0];
                    string after_funcargs = matches[7];
                    if (str_contains("(", till_funcname_starts)) {
                        const string till_funcname_ends = str_split("(", tail)[0];
                        vector<string> splits = str_split(" ", till_funcname_ends);
                        while(!splits.empty() && str_trim(splits[splits.size() - 1]) == "")
                            splits.pop_back();
                        if (splits.empty())
                            throw ERROR("Parse error - expects a function name: " COLOR_FILENAME + hFile + COLOR_DEFAULT);
                        funcname = splits[splits.size() - 1];
                        splits.pop_back();
                        till_funcname_starts = str_concat(splits, " ") + " ";

                        // ---
                        splits = str_split(")", matches[0]);
                        after_funcargs = splits[splits.size() - 1];
                    }

                    const string cleansrc = tpl.empty() ? (
                        "\n" + 
                        remove_func_spec_decl_only_keywords(till_funcname_starts) + 
                        (classes.empty() ? "" : (vector_concat(classes, "::") + "::")) + 
                        funcname + "(" + 
                            remove_func_args_defvals(funcargs) + 
                        ") " + 
                        remove_func_qualifier_decl_only_keywords(after_funcargs)
                    ) : ""; 
                    impl += cleansrc;

                }


                // cout << endl << endl << endl << endl << "-----[ Interface ]-------------------->" << endl;
                // cout << intr << endl;
                // cout << "<----[ /Interface ]-----" << endl << endl;
                // cout << "-----[ Implementation ]-------------------->" << endl;
                // cout << impl << endl;
                // cout << "<----[ /Implementation ]-----" << endl;


                tail = "";  
                continue;
            }

            block_level* lst_level = levels.size() >= 1 ? &levels[levels.size() - 1] : nullptr;
            if (lst_level && lst_level->type == FUNCTION && !lst_level->is_template) 
                impl += tail;  
            else 
                intr += tail;
            tail = ""; 
            continue;
        }

        if (c == '}') {
            if (levels.empty()) 
                throw ERROR("Invalid block levels: " COLOR_FILENAME + hppFile + COLOR_DEFAULT "\n" + tail);

            block_level lst_level = levels[levels.size() - 1];
            if (lst_level.level == block) 
                levels.pop_back();
            block--;  

            // cout << endl << endl << endl << endl;
            // cout << "***************************************************************" << endl;
            // cout << "****level: " << lst_level.level << endl;
            // cout << "****type: " << lst_level.type << endl;
            // cout << "****matches:\n****match:" << vector_concat(lst_level.matches, "\n****match:") << endl;
            // cout << "****head:\n" << lst_level.head << endl;
            // cout << "****tail:\n" << tail << endl;

            switch (lst_level.type) {
                case NAMESPACE:
                    intr += tail;
                    impl += "\n}";
                    tail = ""; 
                    break;
                
                case CLASS_STRUCT:
                    intr += tail;
                    tail = ""; 
                    break;
                
                case FUNCTION:
                    if (lst_level.is_template)
                        intr += tail;
                    else
                        impl += tail;
                    tail = ""; 
                    break;
            
                default:
                    throw ERROR("Invalid code block type");
            }

            // cout << endl << endl << endl << endl << "-----[ Interface ]-------------------->" << endl;
            // cout << intr << endl;
            // cout << "<----[ /Interface ]-----" << endl << endl;
            // cout << "-----[ Implementation ]-------------------->" << endl;
            // cout << impl << endl;
            // cout << "<----[ /Implementation ]-----" << endl;

            continue;
        }
    }
    intr += tail;

    // cout << endl << endl;
    // cout << endl << "======================================================" << endl;
    // cout << hppFile << " (Interface) =>      " << hFile << endl;
    // cout << intr;
    // cout << endl << "======================================================" << endl;
    // cout << hppFile << " (Implementation) => " << cppFile << endl;
    // cout << impl;  
    // cout << endl;  
    // cout << endl;

    fileContents.push_back({ hFile, intr });
    fileContents.push_back({ cppFile, impl });

    // const string hFilePath = path_extract(hFile);
    // if (!file_exists(hFilePath)) file_create_path(hFilePath);
    // file_put_contents(hFile, intr);

    // const string cppFilePath = path_extract(cppFile);
    // if (!file_exists(cppFilePath)) file_create_path(cppFilePath);
    // file_put_contents(cppFile, impl);

    // return hFilename;
}

// vector<string> 
void hppcut_file(vector<FileContent>& fileContents, const args_t& args) {
    const string inputFilename = args_get_input_filename(args);
    const vector<string> cppExtensions = args_get_cpp_extensions(args);
    const string hExtension = args_get_h_extension(args);
    const string hppExtension = args_get_hpp_extension(args);
    const string outputFolder = args_get_output_folder(args);
    const string hppCutFolder = args_get_hpp_cut_folder(args);
    const string hppcutOutputFolder = path_normalize(outputFolder + "/" + hppCutFolder);

    if (!vector_contains(cppExtensions, file_get_extension(inputFilename)))
        throw ERROR(
            "Invalid file extension: " COLOR_FILENAME + inputFilename + COLOR_DEFAULT 
            ", only files with extensions: ." + vector_concat(cppExtensions, ", .")
        );

    vector<string> deps;
    cout << __FILE_LINE__ << " - inputFilename: " << inputFilename << endl;
    csrc_collect_deps(
        path_extract(inputFilename), filename_extract(inputFilename), 
        deps, hExtension, cppExtensions
    );
    vector<string> hppFiles;
    for (const string& dep: deps)
        if (file_get_extension(dep) == hppExtension) hppFiles.push_back(dep);
    
    // vector<string> hFiles;
    for (const string& hppFile: hppFiles) {
        cout << __FILE_LINE__ << " - hppFile: " << hppFile << endl;
        // hFiles.push_back(
            hppcut_file(fileContents, hppFile, hppcutOutputFolder, hExtension, cppExtensions[0]);
        // );
    }
    // return hFiles;

    const string hppcutOutputFolderInputFilename = hppcutOutputFolder + "/" + inputFilename;
    const bool modified = !(
        file_exists(hppcutOutputFolderInputFilename) &&
            file_get_mtime(hppcutOutputFolderInputFilename) >= file_get_mtime(inputFilename)
    );
    // if (modified) cout << "File modified: " COLOR_FILENAME << inputFilename << COLOR_DEFAULT << endl;
    fileContents.push_back({ 
        hppcutOutputFolderInputFilename, 
        modified ? file_get_contents(inputFilename) : "",
        modified
    });


    for (const FileContent& fileContent: fileContents)
        if (file_get_extension(fileContent.filename) == hExtension) {
            const string hFilename = filename_extract(fileContent.filename);
            const string hppFilename = file_replace_extension(hFilename, hppExtension);
            for (FileContent& _fileContent: fileContents) {
                if (!_fileContent.modified) continue;
                _fileContent.content = str_replace(
                    _fileContent.content, 
                    "\"" + hppFilename + "\"", "\"" + hFilename + "\""
                );
                _fileContent.content = str_replace(
                    _fileContent.content, 
                    "/" + hppFilename + "\"", "/" + hFilename + "\""
                );
            }
        }
            
            // const string hFile = fileContent.filename;
            // const string hppFile = file_replace_extension(hFile, hppExtension);
            // const string hppFilename = filename_extract(hppFile);
            // const string hFilename = filename_extract(hFile);
            // hppcut_replace_include_hpp_to_h(hppCutOutputFolder, inputFilename, hppFilename, hFilename);
            // for (const string& _hFile: hFiles)
            //     hppcut_replace_include_hpp_to_h(hppCutOutputFolder, _hFile, hppFilename, hFilename);

}

//vector<string> 
void hppcut_files(vector<FileContent>& fileContents, const args_t& args, const string& inputFolder) {
    const vector<string> cppExtensions = args_get_cpp_extensions(args);

    const vector<string> inputFilenames = file_find_by_extensions(inputFolder, cppExtensions);
    args_t prj_args = args;
    // vector<string> hFiles;
    for (const string& inputFilename: inputFilenames) {
        prj_args["input-filename"] = inputFilename;
        //vector<string> moreHFiles = 
        hppcut_file(fileContents, prj_args);
        //hFiles.insert(hFiles.end(), moreHFiles.begin(), moreHFiles.end());
    }

    // return hFiles;
}

// void hppcut_replace_include_hpp_to_h(
//     const string& hppCutOutputFolder,
//     const string& sourceFile,
//     const string& hppFilename,
//     const string& hFilename
// ) {
//     const string hFilePath = path_normalize(hppCutOutputFolder + "/" + sourceFile);
//     string source = file_get_contents(hFilePath);
//     source = str_replace(source, "\"" + hppFilename + "\"", "\"" + hFilename + "\"");
//     source = str_replace(source, "/" + hppFilename + "\"", "/" + hFilename + "\"");
//     file_put_contents(hFilePath, source);
// }

void hppcut(const args_t& args) {
    // const bool debug = args_get_debug(args);
    const string inputFilename = args_get_input_filename(args);
    // const string hppExtension = args_get_hpp_extension(args);
    // const string hppCutFolder = args_get_hpp_cut_folder(args);
    // const string outputFolder = args_get_output_folder(args);

    // if (debug) { // TODO: it may not needed or user decide?
    //     cout << COLOR_INFO "debug mode, hpp cut skipped.." COLOR_DEFAULT << endl;
    //     return;
    // }
    
    // vector<string> hFiles;
    vector<FileContent> fileContents; // TODO: parse and collect only the changed .hpp file related .h and .cpp files
    if (is_dir(inputFilename)) //hFiles = 
        hppcut_files(fileContents, args, inputFilename);
    else //hFiles = 
        hppcut_file(fileContents, args);


    // const string hppCutOutputFolder = path_normalize(outputFolder + "/" + hppCutFolder);
    for (const FileContent& fileContent: fileContents) {
        if (fileContent.modified)
            cout << "Writing: " COLOR_FILENAME << fileContent.filename << COLOR_DEFAULT << endl;
        fileContent.save();
    }

    // TODO:
    // const string hppCutOutputFolder = path_normalize(outputFolder + "/" + hppCutFolder);
    // file_copy(inputFilename, hppCutOutputFolder + "/" + inputFilename);

    // for (const string& hFile: hFiles) {
    //     const string hppFile = file_replace_extension(hFile, hppExtension);
    //     const string hppFilename = filename_extract(hppFile);
    //     const string hFilename = filename_extract(hFile);
        
    //     hppcut_replace_include_hpp_to_h(hppCutOutputFolder, inputFilename, hppFilename, hFilename);
    //     for (const string& _hFile: hFiles)
    //         hppcut_replace_include_hpp_to_h(hppCutOutputFolder, _hFile, hppFilename, hFilename);
        
    // }

        
}