#!/usr/bin/perl -w

use strict;
use Cwd;
use File::Type;
use File::Copy;
use File::Basename;
use File::Path;

unshift(@INC, $ENV{DPO2_CORE_ROOT} . "/scripts");
require DPOPostbuild;


my $root_dir = $ENV{"BASICEVENTTRANSRESIMPL_PRJ_ROOT"};
$root_dir =~ s/\\/\//g;

# read version of the project
my $major=0;
my $minor=0;
my $patch=0;
if (!DPOUtils::read_project_version($root_dir,
                                    \$major,
                                    \$minor,
                                    \$patch))
{
    print "Can not extract project version from $root_dir.\n";
    exit -1;
}

print "(postbuild.pl) version = $major.$minor.$patch\n";

# Copy dyn lib in $BASICEVENTHANDLING_ROOT/bin and $BASICEVENTHANDLING_ROOT/bin/$(ConfigurationName)
my $ft = File::Type->new();

my $basic_event_handling_root = $ENV{BASICEVENTHANDLING_PRJ_ROOT};
my $debug_dir = "$root_dir/lib/Debug";
my $release_dir = "$root_dir/lib/Release";
my $target_dir = "$basic_event_handling_root/bin";
my $debug_target_dir = "$target_dir/Debug";
my $release_target_dir = "$target_dir/Release";

my @files;
if (DPOUtils::TreeScan($root_dir, \@files))
{
    foreach my $f (@files)
    {
        if (DPOUtils::is_dynamic_lib($f, $ft))
        {
            my($filename, $directories, $suffix) = fileparse($f);

            mkpath($target_dir);
            if (!copy($f, "$target_dir/$filename"))
            {
                print "Failed to copy $f to $debug_target_dir/$filename\n";
            }

            if ($f =~ /$debug_dir/)
            {
                mkpath($debug_target_dir);
                print "copy($f, $debug_target_dir/$filename);\n";
                if (!copy($f, "$debug_target_dir/$filename"))
                {
                    print "Failed to copy $f to $debug_target_dir/$filename\n";
                }
            }

            if ($f =~ /$release_dir/)
            {
                mkpath($release_target_dir);
                print "copy($f, $release_target_dir/$filename);\n";
                if (!copy($f, "$release_target_dir/$filename"))
                {
                    print "Failed to copy $f to $release_target_dir/$filename\n";
                }
            }
        }
    }
}

if (!DPOPostbuild::dpo_postbuild("BasicEventTransResImpl"))
{
    exit -1;
}

