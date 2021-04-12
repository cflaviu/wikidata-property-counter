import qbs

CppApplication {
    consoleApplication: true
    cpp.cxxLanguageVersion: "c++17"
    cpp.includePaths: [
        "."
    ]
    files: [
        "JsonReader.cpp",
        "JsonReader.h",
        "main.cpp",
        "wikidata.json",
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
