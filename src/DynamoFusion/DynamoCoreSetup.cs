using System;
using System.Diagnostics;
using System.Windows;
using Dynamo.Controls;
using Dynamo.Models;
using Dynamo.ViewModels;
using Dynamo.Logging;
using Dynamo.Core;
using Dynamo.Wpf.ViewModels.Watch3D;
using Dynamo.Applications;
using DynamoShapeManager;
using System.Runtime.InteropServices;
using System.Reflection;
using System.IO;
using Dynamo.Scheduler;

namespace DynamoFusion
{
    public class DynamoCoreSetup
    {
        private DynamoViewModel viewModel = null;
        private string commandFilePath;

        [DllImport("msvcrt.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int _putenv(string env);

        public DynamoCoreSetup(string[] args)
        {
            var cmdLineArgs = StartupUtils.CommandLineArguments.Parse(args);
            var locale = StartupUtils.SetLocale(cmdLineArgs);
            _putenv(locale);
            commandFilePath = cmdLineArgs.CommandFilePath;
        }

        public void RunApplication(string asmLocation)
        {
            try
            {
                var model = Dynamo.Applications.StartupUtils.MakeModel(false, asmLocation);

                var viewModel = DynamoViewModel.Start(
                    new DynamoViewModel.StartConfiguration()
                    {
                        CommandFilePath = string.Empty,
                        DynamoModel = model,
                        Watch3DViewModel = HelixWatch3DViewModel.TryCreateHelixWatch3DViewModel(new Watch3DViewModelStartupParams(model), model.Logger),
                        ShowLogin = true
                    });

                var view = new DynamoView(viewModel);

                //
                // app is needed when testing through console applicaiton.
                //
                //var app = new Application();
                //app.Run(view);
                view.Show();

            }

            catch (Exception e)
            {
                try
                {
#if DEBUG
                    // Display the recorded command XML when the crash happens, 
                    // so that it maybe saved and re-run later
                    if (viewModel != null)
                        viewModel.SaveRecordedCommand.Execute(null);
#endif

                    DynamoModel.IsCrashing = true;
                    //InstrumentationLogger.LogException(e);
                    //StabilityTracking.GetInstance().NotifyCrash();

                    if (viewModel != null)
                    {
                        // Show the unhandled exception dialog so user can copy the 
                        // crash details and report the crash if she chooses to.
                        viewModel.Model.OnRequestsCrashPrompt(null,
                            new CrashPromptArgs(e.Message + "\n\n" + e.StackTrace));

                        // Give user a chance to save (but does not allow cancellation)
                        viewModel.Exit(allowCancel: false);
                    }
                }
                catch
                {
                }

                Debug.WriteLine(e.Message);
                Debug.WriteLine(e.StackTrace);
            }
        }

    }
}
