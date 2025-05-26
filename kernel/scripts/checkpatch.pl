#!/usr/bin/perl
# CTRLxT_STUDIOS Omni-Kernel-Prime Code Style Checker
# Based on Linux kernel checkpatch.pl

use strict;
use warnings;
use Getopt::Long;

my $quiet = 0;
my $tree = 1;
my $chk_signoff = 1;
my $chk_patch = 1;
my $tst_only = 0;
my $emacs = 0;
my $terse = 0;
my $file = 0;
my $no_warnings = 0;
my $ignore_perl_version = 0;
my $show_types = 0;
my $root;
my %debug = ();
my $help = 0;
my $configuration_file = ".checkpatch.conf";

sub help {
    my ($exitcode) = @_;

    print << "EOM";
Usage: $P [OPTION]... [FILE]...
Version: 1.0

Options:
  -q, --quiet                quiet mode
  --no-tree                  run without a kernel tree
  --no-signoff               do not check for 'Signed-off-by' line
  --patch                    treat FILE as patchfile (default)
  --emacs                    emacs compile window format
  --terse                    one line per report
  -f, --file                 treat FILE as regular source file
  --no-warnings              do not show warnings
  --ignore-perl-version      override version checks
  --show-types               show the type of the message
  --root=PATH                PATH to the kernel tree root
  --debug KEY=[0|1]          turn on/off debugging of KEY, where KEY is one of
                             'values', 'possible', 'type', and 'attr' (default
                             is all off)
  --test-only=WORD           report only warnings/checks matching WORD
  -h, --help, --version      display this help and exit

When FILE is - read standard input.
EOM

    exit($exitcode);
}

GetOptions(
    'q|quiet+' => \$quiet,
    'no-tree!' => \$tree,
    'no-signoff!' => \$chk_signoff,
    'patch!' => \$chk_patch,
    'emacs!' => \$emacs,
    'terse!' => \$terse,
    'f|file!' => \$file,
    'no-warnings!' => \$no_warnings,
    'ignore-perl-version!' => \$ignore_perl_version,
    'show-types!' => \$show_types,
    'root=s' => \$root,
    'debug=s' => \%debug,
    'test-only=s' => \$tst_only,
    'h|help|version' => \$help,
) or help(1);

help(0) if ($help);

# Check for required perl version
if (!$ignore_perl_version) {
    require 5.10.0;
}

# Check for required modules
eval {
    require Text::Tabs;
    require Term::ANSIColor;
};
if ($@) {
    print "Required perl modules are missing.\n";
    print "Please install the following perl modules:\n";
    print "  Text::Tabs\n";
    print "  Term::ANSIColor\n";
    exit(1);
}

# Process files
foreach my $filename (@ARGV) {
    if ($filename eq '-') {
        $filename = 'STDIN';
    }
    
    if (!open(my $fh, '<', $filename)) {
        print "Cannot open $filename: $!\n";
        next;
    }
    
    my $linenr = 0;
    my $prevline = "";
    my $stashline = "";
    my $clean = 1;
    
    while (my $line = <$fh>) {
        $linenr++;
        
        # Check for common style issues
        if ($line =~ /^\s*$/ && $prevline =~ /^\s*$/) {
            print "WARNING: Multiple blank lines at line $linenr\n";
            $clean = 0;
        }
        
        if ($line =~ /^[ \t]+$/) {
            print "WARNING: Line contains only whitespace at line $linenr\n";
            $clean = 0;
        }
        
        if ($line =~ /\t/) {
            print "WARNING: Line contains tabs at line $linenr\n";
            $clean = 0;
        }
        
        if ($line =~ /[ \t]$/) {
            print "WARNING: Line has trailing whitespace at line $linenr\n";
            $clean = 0;
        }
        
        if ($line =~ /^[ \t]+#/) {
            print "WARNING: Comment should start with # at line $linenr\n";
            $clean = 0;
        }
        
        $prevline = $line;
    }
    
    close($fh);
    
    if ($clean) {
        print "No style issues found in $filename\n";
    }
}

exit(0); 