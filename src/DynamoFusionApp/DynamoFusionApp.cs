using System;
using System.Windows;
using System.Reflection;
using System.IO;
using System.Linq;
using System.Diagnostics;

using Dynamo.Models;
using Dynamo.ViewModels;
using Dynamo.Wpf.ViewModels.Watch3D;
using Dynamo.Controls;

namespace DynamoFusionApp
{
    public class DynamoFusionApp
    {
        private DynamoViewModel dynamoViewModel;
        private static string dynamopath;

        public void Run(string asmLocation)
        {
            AppDomain.CurrentDomain.AssemblyResolve += ResolveAssembly;

            //Include Dynamo Core path in System Path variable for helix to load properly.
            UpdateSystemPathForProcess();

            StartUI(asmLocation);

        }

        #region Fusion to Dynamo Nodes

        public void CreateSelectionNode()
        {
            var command = new DynamoModel.CreateNodeCommand(new[] { Guid.NewGuid() }, "Fusion.SelectEntity", 0, 0, true, false);
            dynamoViewModel.ExecuteCommand(command);
        }

        #endregion
        /// <summary>
        /// Handler to the ApplicationDomain's AssemblyResolve event.
        /// If an assembly's location cannot be resolved, an exception is
        /// thrown. Failure to resolve an assembly will leave Dynamo in 
        /// a bad state, so we should throw an exception here which gets caught 
        /// by our unhandled exception handler and presents the crash dialogue.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="args"></param>
        /// <returns></returns>
        public static Assembly ResolveAssembly(object sender, ResolveEventArgs args)
        {
            var assemblyPath = string.Empty;
            var assemblyName = new AssemblyName(args.Name).Name + ".dll";

            try
            {
                assemblyPath = Path.Combine(DynamoCorePath, assemblyName);
                if (File.Exists(assemblyPath))
                    return Assembly.LoadFrom(assemblyPath);

                var assemblyLocation = Assembly.GetExecutingAssembly().Location;
                var assemblyDirectory = Path.GetDirectoryName(assemblyLocation);

                assemblyPath = Path.Combine(assemblyDirectory, assemblyName);
                return (File.Exists(assemblyPath) ? Assembly.LoadFrom(assemblyPath) : null);
            }
            catch (Exception ex)
            {
                throw new Exception(string.Format("The location of the assembly, {0} could not be resolved for loading.", assemblyPath), ex);
            }
        }
        
        #region private methods
        
        /// <summary>
        /// Returns the path of Dynamo Core installation.
        /// </summary>
        private static string DynamoCorePath
        {
            get
            {
                if (string.IsNullOrEmpty(dynamopath))
                {
                    dynamopath = GetDynamoCorePath();
                }
                return dynamopath;
            }
        }

        /// <summary>
        /// Finds the Dynamo Core path by looking into registery or potentially a config file.
        /// </summary>
        /// <returns>The root folder path of Dynamo Core.</returns>
        private static string GetDynamoCorePath()
        {
            var assembly = Assembly.GetExecutingAssembly();
            var version = assembly.GetName().Version;

            var installs = DynamoInstallDetective.DynamoProducts.FindDynamoInstallations(Path.GetDirectoryName(assembly.Location));
            if (installs == null) return string.Empty;

            return installs.Products
                .Where(p => p.VersionInfo.Item1 == version.Major && p.VersionInfo.Item2 == version.Minor)
                .Select(p => p.InstallLocation)
                .FirstOrDefault();
        }

        /// <summary>
        /// Add Dynamo Core location to the PATH system environment variable.
        /// This is to make sure dependencies (e.g. Helix assemblies) can be located.
        /// </summary>
        private static void UpdateSystemPathForProcess()
        {
            var path =
                    Environment.GetEnvironmentVariable(
                        "Path",
                        EnvironmentVariableTarget.Process) + ";" + DynamoCorePath;
            Environment.SetEnvironmentVariable("Path", path, EnvironmentVariableTarget.Process);
        }

        private void StartUI(string asmLocation)
        {
            try
            {
                var model = Dynamo.Applications.StartupUtils.MakeModel(false, asmLocation);

                dynamoViewModel = DynamoViewModel.Start(
                    new DynamoViewModel.StartConfiguration()
                    {
                        CommandFilePath = string.Empty,
                        DynamoModel = model,
                        Watch3DViewModel = HelixWatch3DViewModel.TryCreateHelixWatch3DViewModel(new Watch3DViewModelStartupParams(model), model.Logger),
                        ShowLogin = true
                    });

                var view = new DynamoView(dynamoViewModel);
                view.Show();
            }
            catch (Exception e)
            {
                try
                {
#if DEBUG
                    // Display the recorded command XML when the crash happens, 
                    // so that it maybe saved and re-run later
                    if (dynamoViewModel != null)
                        dynamoViewModel.SaveRecordedCommand.Execute(null);
#endif

                    //DynamoModel.IsCrashing = true;
                    //InstrumentationLogger.LogException(e);
                    //StabilityTracking.GetInstance().NotifyCrash();

                    //if (viewModel != null)
                    //{
                    //    // Show the unhandled exception dialog so user can copy the 
                    //    // crash details and report the crash if she chooses to.
                    //    viewModel.Model.OnRequestsCrashPrompt(null,
                    //        new CrashPromptArgs(e.Message + "\n\n" + e.StackTrace));

                    //    // Give user a chance to save (but does not allow cancellation)
                    //    viewModel.Exit(allowCancel: false);
                    //}
                }
                catch
                {
                }

                Debug.WriteLine(e.Message);
                Debug.WriteLine(e.StackTrace);
            }
        }
        
        #endregion
    }
}
