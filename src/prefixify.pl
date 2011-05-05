j!/usr/bin/perl -w

my @files = (
    "t4k-audio.c",
    "t4k-loaders.c",
    "t4k-main.c",
    "t4k-menu.c",
    "t4k-pixels.c",
    "t4k-sdl.c"
);

print "Enter unprefixed function name or 'bye':\n";
while ($regex = <STDIN>) {
    exit if ($regex =~ m/^(bye|exit|q|q$)/);
    $regex =~ s/\s+$//; #trim trailing whitespace
    push(@regexes, $regex); #add to list of replacement candidates
    print "Enter unprefixed function name or 'bye':\n";
}

foreach $file (@files) {
    open(INFILE, $file) or die ("Can't open $file");
    open(OUTFILE, ">", "$file.new") or die ("Can't write new file - $!\n");
    my $matches = 0;
    print "Processing $file...";
    while (my $line = <INFILE>) {
	foreach $regex (@regexes) {
	    if ($line =~ m#[^\w]($regex)\(#) {
		#print ":$line\n";
		if ($line =~ s#$regex\(#T4K_$regex\(#g ) {
		    $matches++;
		}
	    }
	}
	print OUTFILE "$line";
    }
    print "$matches replacements\n";
    close INFILE;
    close OUTFILE;
}	

print "Done";
