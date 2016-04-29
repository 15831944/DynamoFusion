# DynamoFusion

1. Copy Dynamo Core binaries to src\cpp_bin\x64\$(Platform).
    Resolve Assembly is still not working.
2. Load up DynamoFusion.sln and build projects DynamoFusion & DynamoFusionWrapper.
    DynamoFusion is currently Sandbox.
    DynamoFusionWrapper is a C++ wrapper for DynamoFusion.
3. Load up DynamoAddin.sln and build it.
    This is where Fusion customization is done.
4. Run CopyToDynamoAddinBin-$(Platform).bat to copy neccessary files to DynamoAddin_bin folder.
5. Copy files in DynamoAddin_bin to Fusion360 Addin folder.
