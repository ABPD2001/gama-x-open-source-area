#ifndef HELP_HPP
#define HELP_HPP

constexpr char *HELP_TXT = R"(
Basic Usage
    gxf [verb] [arguments/flags]...

Verbs:
    snipper
        Adds data-containing headers and footers to raw data sets. It can also inject metadata into individual inputs using the '-S' flag, while supporting sectioning through multiple inputs and configuration files.
        Flags:
            [-A, --alignment]:                      align each snippet (with header and footer) to a specific byte-alignment.
            # ===> [gxf snipper -A 4 ...]
            
            [-b, --binary]:                         inputs and output are in binary, or in other words, work with binaries.
            [-B, --binary-big-endian]:              inputs and output are in big-endian binary.
            [-C, --config]:                         configuration file path, that includes header/footer format, header/footer section fields values (format of it will be explained more in further).
            [-S, --meta-snipping]:                  act as a metadata injecter.
            
            [-fc, --footer-seperator-char]:         define what character should split snippet (section) content and footer.
            # ===> [gxf snipper -fc a ...]
            
            [-hc, --header-seperator-char]:         define what character should split snippet (section) content and header.
            [-p, --padding-fillter-char]:           define which character should used as alignment padding.
            
            [-Afc, --footer-seperator-char-ascii]:  define what byte in ascii table should split snippet (section) content and footer.
            # ===> [gxf snipper -Afc 65 ...]
            
            [-Ahc, --footer-seperator-char-ascii]:  define what byte in ascii table should split snippet (section) content and header.
            [-Ap, --padding-fillter-char-ascii]:    define which byte in ascii table should used as alignment padding.

        
        Basic Usage:
            gxf snipper [FLAGS/INPUT FILES]...

            [gxf snipper -C my_config.conf data1.txt data2.txt data3.txt -o output.txt]
            [gxf snipper -b -C my_config.conf data1.bin data2.bin -o output.bin]
            [gxf snipper -B -C my_config.conf data1.bin data2.bin data3.bin data4.bin -o output.bin]


        caster
            Converts raw data from one format to another. Multiple output formats can be specified using the '-m' flag.
            Flags:
                [-m, --multi-cast]:     instead of an atomic format conversion, convert a input file to multiple output formats, takes numeric-value as number of output formats.
                # ===> [gxf caster -m 4 ...]

                [-b, --binary]:                         input file is a binary data.
                [-B, --binary-big-endian]:              input file is a big-endian binary data.
                [-f, --format-logic]:                   input file format/logic config file destinatoin, (note: it's based on '-l' or '-f' flags of Gama-X Compiler 'gx').
                [-c, --seperator-char]:                 it can be used istead of '-f' flag, only works when input format is text, reads input seperated within entered char.
                [-Ac, --seperator-char-ascii]:  it      can be used istead of '-f' flag, only works when input format is text, reads input seperated within entered byte in ascii table.
                
                [-o, --output]:                         output file(s) destination.
                [-tb, --target-binary]:                 output file(s) is a binary data.
                [-tB, --target-binary-big-endian]:      output file(s) is a big-endian binary data.
                [-tf, --target-format-logic]:           output file(s) format/logic config file destinatoin, (note: it's based on '-l' or '-f' flags of Gama-X Compiler 'gx').
                [-tc, --target-seperator-char]:         it can be used istead of '-f' flag, only works when input format is text, writes output(s) seperated within entered char.
                [-Atc, --target-seperator-char-ascii]:  it can be used istead of '-f' flag, only works when input format is text, writes output(s) seperated within entered byte in ascii table.
                
                Important: it's required to enter output-related flags multi-values that seperated with ',' (comma) if '-m, --multi-cast' flag is valued, order of all output-related flags should be same.
                
                Basic Usage:
                    gxf caster [FLAGS/FILE]...

                    [gxf caster data.bin -b -o new_data.bin]
                    [gxf caster data.txt -m 3 -o data1.txt,data2.txt,data3.txt -c e,a,#]
                    [gxf caster data.txt -m 2 -o data1.txt,data2.txt -Ac 78,65]
        packeter
)";
#endif