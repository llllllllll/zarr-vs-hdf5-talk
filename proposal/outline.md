Zarr is a modern library and data format for storing chunked, compressed N-dimensional data.
HDF5 is a mature (20 years old) library and data format which is also designed to handle chunked compressed N-dimensional data.
This talk will attempt to compare these two similar technologies, allowing the audience to pick the correct tool for their problem.

This talk will begin by covering basic usage of both zarr and h5py, a Python interface to HDF5.
Zarr and h5py have a nearly identical interface, so in many cases you can swap libraries without requiring any code changes.
In this section we will cover some of the core concepts behind working with chunked multidimensional data.

The first important difference between zarr and HDF5 that we will cover is their handling of compressors and filters.
Compressors and filters are how both zarr and HDF5 implement compression and other data transformations like checksumming.
In both libraries, compressors and filters are composable units which can form a _filter pipeline_ that transparently acts on a dataset.
This talk will cover both libraries' built in filters, as well as how they each support user-defined extensions.

Next, we will cover the differences in how data is stored in both libraries.
Both zarr and HDF5 provide multiple concrete storage types, ranging from a single file on local disk, to distributed files across a cloud object store like Amazon S3.
This talk will cover the default and built in formats for both zarr and HDF5.
This talk will also cover how zarr and HDF5 can be extended to support new storage formats.

While both zarr and HDF5 can be used with the same data, the choice of filters, storage, and other implementation details can cause large differences in performance.
Throughout this talk, profiling methods will be shown to help the audience design and run their own experiments to compare zarr and HDF5 with their own data.

Before the talk, audience members should:

- Know Python syntax and vocabulary.
- Know basic numpy usage, including working with multidimensional arrays.

By the end of this talk, audience members will:

- Know the basic use cases for zarr or HDF5.
- Understand the basics of filters and compressors in zarr and HDF5.
- Understand the basics of data storage in zarr and HDF5.
- Be familiar with how zarr and HDF5 can be extended.
- Be able to begin comparing zarr and HDF5 with their own data.
