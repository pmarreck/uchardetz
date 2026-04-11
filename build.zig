const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const lib_sources: []const []const u8 = &.{
        "src/CharDistribution.cpp",
        "src/JpCntx.cpp",
        "src/LangModels/LangArabicModel.cpp",
        "src/LangModels/LangBulgarianModel.cpp",
        "src/LangModels/LangDanishModel.cpp",
        "src/LangModels/LangEsperantoModel.cpp",
        "src/LangModels/LangFrenchModel.cpp",
        "src/LangModels/LangGermanModel.cpp",
        "src/LangModels/LangGreekModel.cpp",
        "src/LangModels/LangHebrewModel.cpp",
        "src/LangModels/LangHungarianModel.cpp",
        "src/LangModels/LangRussianModel.cpp",
        "src/LangModels/LangSpanishModel.cpp",
        "src/LangModels/LangThaiModel.cpp",
        "src/LangModels/LangTurkishModel.cpp",
        "src/LangModels/LangVietnameseModel.cpp",
        "src/nsHebrewProber.cpp",
        "src/nsCharSetProber.cpp",
        "src/nsBig5Prober.cpp",
        "src/nsEUCJPProber.cpp",
        "src/nsEUCKRProber.cpp",
        "src/nsEUCTWProber.cpp",
        "src/nsEscCharsetProber.cpp",
        "src/nsEscSM.cpp",
        "src/nsGB2312Prober.cpp",
        "src/nsMBCSGroupProber.cpp",
        "src/nsMBCSSM.cpp",
        "src/nsSBCSGroupProber.cpp",
        "src/nsSBCharSetProber.cpp",
        "src/nsSJISProber.cpp",
        "src/nsUTF8Prober.cpp",
        "src/nsLatin1Prober.cpp",
        "src/nsUniversalDetector.cpp",
        "src/uchardet.cpp",
    };

    const cpp_flags: []const []const u8 = &.{
        "-DVERSION=\"0.0.6\"",
        "-Wall",
    };

    // Static library
    // Disable C sanitizer: the C++ code uses enum values beyond the defined
    // range (valid C++ but caught by Zig's stricter runtime checks).
    const lib_mod = b.createModule(.{
        .target = target,
        .optimize = optimize,
        .link_libcpp = true,
        .sanitize_c = .off,
    });
    lib_mod.addCSourceFiles(.{
        .files = lib_sources,
        .flags = cpp_flags,
    });
    lib_mod.addIncludePath(b.path("src"));

    const lib = b.addLibrary(.{
        .linkage = .static,
        .name = "uchardet",
        .root_module = lib_mod,
    });
    b.installArtifact(lib);

    // Shared library
    const shared_mod = b.createModule(.{
        .target = target,
        .optimize = optimize,
        .link_libcpp = true,
        .sanitize_c = .off,
    });
    shared_mod.addCSourceFiles(.{
        .files = lib_sources,
        .flags = cpp_flags,
    });
    shared_mod.addIncludePath(b.path("src"));

    const shared_lib = b.addLibrary(.{
        .linkage = .dynamic,
        .name = "uchardet",
        .root_module = shared_mod,
    });
    b.installArtifact(shared_lib);

    // Install the public C header
    b.installFile("src/uchardet.h", "include/uchardet/uchardet.h");

    // CLI executable
    const exe_mod = b.createModule(.{
        .target = target,
        .optimize = optimize,
        .link_libcpp = true,
    });
    exe_mod.addCSourceFiles(.{
        .files = &.{"src/tools/uchardet.cpp"},
        .flags = cpp_flags,
    });
    exe_mod.addIncludePath(b.path("src"));
    exe_mod.linkLibrary(lib);

    const exe = b.addExecutable(.{
        .name = "uchardet",
        .root_module = exe_mod,
    });
    b.installArtifact(exe);

    // Run step
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }
    const run_step = b.step("run", "Run the uchardet CLI");
    run_step.dependOn(&run_cmd.step);

    // Test executable
    const test_mod = b.createModule(.{
        .target = target,
        .optimize = optimize,
        .sanitize_c = .off,
    });
    test_mod.addCSourceFiles(.{
        .files = &.{"test/uchardet-tests.c"},
        .flags = &.{"-Wall"},
    });
    test_mod.addIncludePath(b.path("src"));
    test_mod.linkLibrary(lib);

    const test_exe = b.addExecutable(.{
        .name = "uchardet-tests",
        .root_module = test_mod,
    });

    // Test step: run the test binary against each test file.
    // Known-broken tests (from upstream CMakeLists.txt) are skipped.
    const test_step = b.step("test", "Run charset detection tests");
    const test_files: []const []const u8 = &.{
        "test/ar/iso-8859-6.txt",
        "test/ar/utf-8.txt",
        "test/ar/windows-1256.txt",
        "test/bg/windows-1251.txt",
        // "test/da/iso-8859-1.txt",    // known broken upstream
        // "test/da/iso-8859-15.txt",   // known broken upstream
        "test/da/utf-8.txt",
        "test/da/windows-1252.txt",
        "test/de/iso-8859-1.txt",
        "test/de/windows-1252.txt",
        "test/el/iso-8859-7.txt",
        "test/el/utf-8.txt",
        "test/el/windows-1253.txt",
        "test/en/ascii.txt",
        "test/eo/iso-8859-3.txt",
        "test/es/iso-8859-1.txt",
        // "test/es/iso-8859-15.txt",   // known broken upstream
        "test/es/utf-8.txt",
        "test/es/windows-1252.txt",
        "test/fr/iso-8859-1.txt",
        "test/fr/iso-8859-15.txt",
        "test/fr/utf-16.be",
        "test/fr/utf-32.le",
        "test/fr/utf-8.txt",
        "test/fr/windows-1252.txt",
        // "test/he/iso-8859-8.txt",    // known broken upstream
        "test/he/utf-8.txt",
        "test/he/windows-1255.txt",
        "test/hu/iso-8859-2.txt",
        "test/hu/windows-1250.txt",
        "test/ja/euc-jp.txt",
        "test/ja/iso-2022-jp.txt",
        "test/ja/shift_jis.txt",
        // "test/ja/utf-16be.txt",      // known broken upstream
        // "test/ja/utf-16le.txt",      // known broken upstream
        "test/ja/utf-8.txt",
        "test/ko/euc-kr.smi",
        "test/ko/iso-2022-kr.txt",
        "test/ko/utf-16.le",
        "test/ko/utf-32.be",
        "test/ko/utf-8.txt",
        "test/ru/ibm855.txt",
        "test/ru/ibm866.txt",
        "test/ru/iso-8859-5.txt",
        "test/ru/koi8-r.txt",
        "test/ru/mac-cyrillic.txt",
        "test/ru/windows-1251.txt",
        "test/th/iso-8859-11.txt",
        "test/th/tis-620.txt",
        "test/th/utf-8.txt",
        "test/tr/iso-8859-3.txt",
        "test/tr/iso-8859-9.txt",
        "test/vi/utf-8.txt",
        "test/vi/viscii.txt",
        "test/vi/windows-1258.txt",
        "test/zh/big5.txt",
        "test/zh/euc-tw.txt",
        "test/zh/gb18030.txt",
        "test/zh/utf-8.txt",
    };

    for (test_files) |test_file| {
        const run = b.addRunArtifact(test_exe);
        run.addFileArg(b.path(test_file));
        test_step.dependOn(&run.step);
    }
}
