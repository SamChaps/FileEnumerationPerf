using System.Collections.Concurrent;
using System.Diagnostics;

// TODO: Enter a valid path to a directory with files.
string path = @"";

var stopwatch = new Stopwatch();
var concurrentBagOfFiles = new ConcurrentBag<FileInfo>();

// Measure time for Parallel.ForEach
stopwatch.Start();
Parallel.ForEach(Directory.EnumerateFiles(path, "*.*", SearchOption.AllDirectories), file => 
{
    concurrentBagOfFiles.Add(new FileInfo(file));
});
stopwatch.Stop();
Console.WriteLine($"Parallel.ForEach took: {stopwatch.ElapsedMilliseconds} ms");
Console.WriteLine($"Number of files: {concurrentBagOfFiles.Count} files");
Console.WriteLine();

// Measure time for regular foreach
stopwatch.Reset();

var listOfFiles = new List<FileInfo>();
stopwatch.Start();
var files = Directory.EnumerateFiles(path, "*.*", SearchOption.AllDirectories);
foreach (var file in files)
{
    var fileInfo = new FileInfo(file);

    listOfFiles.Add(fileInfo);
}
stopwatch.Stop();

Console.WriteLine($"Directory.EnumerateFiles took: {stopwatch.ElapsedMilliseconds} ms");
Console.WriteLine($"Number of files: {listOfFiles.Count} files");

stopwatch.Reset();
