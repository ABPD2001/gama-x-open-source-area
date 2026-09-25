```cpp
#ifndef HELP_HPP
#define HELP_HPP

    constexpr const char *HELP_TXT = R"(
Basic Usage
    gxf [VERB] [ARGUMENTS/FLAGS]...

'gxf' is the official Gama-X output formatter. It applies post-processing
transformations to Gama-X output without modifying the processing stage.
Supported operations include metadata injection, sectioning, output merging,
chunking, and format conversion.

Verbs:
    help
        Print this help text.

    version
        Print the software version.

    snipper
        Add data-containing headers and footers to raw data sets. Metadata
        injection can be performed on individual inputs using the '-S' flag.
        Sectioning is supported through multiple inputs and configuration files.

        Flags:
            [-A, --alignment]
                Align each snippet, including its header and footer, to the
                specified byte alignment.

                Example:
                    gxf snipper -A 4 ...

            [-b, --binary]
                Treat input and output as binary data.

            [-B, --binary-big-endian]
                Treat input and output as big-endian binary data.

            [-C, --config]
                Specify the configuration file containing header/footer
                formats and section field values.

                Configuration file format:

                    [HEADER FORMAT](format...)
                    [FOOTER FORMAT](format...)

                    [HEADER]
                    fields...
                    fields...
                    ...

                    [FOOTER]
                    fields...
                    fields...
                    ...

                Lines beginning with '#' are treated as comments.

                Example (.txt):

                    [HEADER FORMAT](size=<SIZE>,name=<NAME>,id=<IDX>)
                    [FOOTER FORMAT]()

                    [HEADER]
                    NAME=my_app
                    NAME=my_app2

                    [FOOTER]
                    # No footer fields.

                Tags enclosed in '<...>' are replaced with predefined values
                or fields defined in the HEADER and FOOTER sections.

                Predefined tags:

                    <SIZE>      Size of the snippet context.
                    <UNIX_MS>   Current Unix timestamp in milliseconds.
                    <IDX>       Zero-based snippet index.

                Binary format:

                    [HEADER FORMAT](string,u64,s64,u16,u16)
                    [FOOTER FORMAT]()

                    [HEADER]
                    hello,432,-2,23,234
                    hello2,_size_,-234,3,4

                    [FOOTER]
                    # No footer fields.

                HEADER and FOOTER field order must match the order specified
                by their corresponding format definitions.

                The predefined '_size_' value represents the size of the
                current snippet context.

                Supported binary field types:

                    string      Null-terminated string.
                    text        Non-null-terminated string.
                    uX          Unsigned integer of X bits (u8, u16, u32, u64, ...).
                    sX          Signed integer of X bits (s8, s16, s32, s64, ...).

            [-S, --meta-snipping]
                Enable metadata injection mode.

            [-fc, --footer-separator-char]
                Specify the character used to separate section content
                from the footer.

                Example:
                    gxf snipper -fc a ...

            [-hc, --header-separator-char]
                Specify the character used to separate section content
                from the header.

            [-p, --padding-filler-char]
                Specify the character used as alignment padding.

            [-Afc, --footer-separator-char-ascii]
                Specify the ASCII byte value used to separate section
                content from the footer.

                Example:
                    gxf snipper -Afc 65 ...

            [-Ahc, --header-separator-char-ascii]
                Specify the ASCII byte value used to separate section
                content from the header.

            [-Ap, --padding-filler-char-ascii]
                Specify the ASCII byte value used as alignment padding.


        Basic Usage:
            gxf snipper [FLAGS/INPUT FILES]...

            gxf snipper -C my_config.conf data1.txt data2.txt data3.txt \
                -o output.txt

            gxf snipper -b -C my_config.conf data1.bin data2.bin \
                -o output.bin

            gxf snipper -B -C my_config.conf data1.bin data2.bin \
                data3.bin data4.bin -o output.bin


    caster
        Convert raw data from one format to another. Multiple output formats
        can be generated using the '-m' flag.

        Flags:
            [-m, --multi-cast]
                Instead of performing an atomic format conversion, convert
                an input file to multiple output formats. The argument
                specifies the number of output formats.

                Example:
                    gxf caster -m 4 ...

            [-b, --binary]
                Treat the input as binary data.

            [-B, --binary-big-endian]
                Treat the input as big-endian binary data.

            [-f, --format-logic]
                Specify the input format/logic configuration file.
                The format specification is based on the '-l' and '-f'
                options of the Gama-X compiler ('gx').

            [-c, --separator-char]
                Specify the input separator character instead of using
                '-f'. This option is available only for text input.

            [-Ac, --separator-char-ascii]
                Specify the input separator as an ASCII byte value instead
                of using '-f'. This option is available only for text input.

            [-o, --output]
                Specify the destination file(s).

            [-tb, --target-binary]
                Write output file(s) as binary data.

            [-tB, --target-binary-big-endian]
                Write output file(s) as big-endian binary data.

            [-tf, --target-format-logic]
                Specify the output format/logic configuration file.
                The format specification is based on the '-l' and '-f'
                options of the Gama-X compiler ('gx').

            [-tc, --target-separator-char]
                Specify the output separator character instead of using
                '-tf'. This option is available only for text output.

            [-Atc, --target-separator-char-ascii]
                Specify the output separator as an ASCII byte value instead
                of using '-tf'. This option is available only for text output.

        Multi-Cast:
            When '-m' or '--multi-cast' is specified, all output-related
            options must accept comma-separated values. The number and order
            of values must match the specified number of output formats.

        Basic Usage:
            gxf caster [FLAGS/FILE]...

            gxf caster data.bin -b -o new_data.bin

            gxf caster data.txt -m 3 \
                -o data1.txt,data2.txt,data3.txt \
                -c e,a,#

            gxf caster data.txt -m 2 \
                -o data1.txt,data2.txt \
                -Ac 78,65


    packeter
        Split one or more input files into sequential data packets.
        Packets are intended for partitioning, segmentation, storage,
        and distributed processing while preserving the original raw data.

        Flags:
            [-s, --packet-size]
                Specify the size of each packet in bytes.

                Example:
                    gxf packeter -s 4096 ...
                    # 4 KiB per packet.

            [-o, --output]
                Specify the output filename pattern. The '$' character is
                used as the packet index placeholder.

                Examples:
                    gxf packeter ... -o packet$$$.bin
                    # packet001.bin, packet002.bin, packet003.bin

                    gxf packeter ... -o packet$.bin
                    # packet1.bin, packet2.bin, packet3.bin


        Basic Usage:
            gxf packeter [FLAGS/FILES]...

            gxf packeter large_data.txt -s 512 \
                -o my_packet$$.txt

            gxf packeter large_data.bin large_data2.bin -s 1024 \
                -o my_packet$$$$.bin


V1.0.0
<--- Gama-X Output Formatter --->
)";

#endif
```
