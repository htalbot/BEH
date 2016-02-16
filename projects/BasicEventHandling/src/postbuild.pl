#!/usr/bin/perl -w

use strict;
use Cwd;

unshift(@INC, $ENV{DPO2_CORE_ROOT} . "/scripts");
require DPOPostbuild;


my $root_dir = $ENV{"BASICEVENTHANDLING_PRJ_ROOT"};
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
my $release_dir = "$basic_event_handling_root/lib/Release";
my $debug_dir = "$basic_event_handling_root/lib/Debug";
my $target_dir = "$basic_event_handling_root/bin";

# TO_DO: linux

if (-e "$release_dir/BasicEventHandling.dll")
{
    my $src = "$release_dir/BasicEventHandling.dll";
    my $target = "$target_dir/BasicEventHandling.dll";
    if (!File::Copy::syscopy($src, $target))
    {
        print "Failed to copy $src to $target\n";
    }
}

if (-e "$debug_dir/BasicEventHandlingd.dll")
{
    my $src = "$debug_dir/BasicEventHandlingd.dll";
    my $target = "$target_dir/BasicEventHandlingd.dll";
    if (!File::Copy::syscopy($src, $target))
    {
        print "Failed to copy $src to $target\n";
    }
}

if (!DPOPostbuild::dpo_postbuild("BasicEventHandling"))
{
    exit -1;
}

