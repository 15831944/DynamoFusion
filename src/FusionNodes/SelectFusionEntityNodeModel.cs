using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Dynamo.Controls;
using Dynamo.Graph.Nodes;
using Dynamo.UI.Commands;
using Dynamo.Wpf;
using FusionNodes.Controls;
using Autodesk.DesignScript.Runtime;

namespace FusionNodes
{
    // The NodeName attribute is what will display on 
    // top of the node in Dynamo
    [NodeName("Select Entity Button")]

    // The NodeCategory attribute determines how your
    // node will be organized in the library. You can
    // specify your own category or use one of the 
    // built-ins provided in BuiltInNodeCategories.
    [NodeCategory("DynamoFusion.Fusion")]

    // The description will display in the tooltip
    // and in the help window for the node.
    [NodeDescription("Select Entities in Fusion and port them to Dynamo.")]

    // Add the IsDesignScriptCompatible attribute to ensure
    // that it gets loaded in Dynamo.
    [IsDesignScriptCompatible]
    public class SelectFusionEntityNodeModel : NodeModel
    {
        #region private members

        #endregion

        #region properties

        /// <summary>
        /// DelegateCommand objects allow you to bind
        /// UI interaction to methods on your data context.
        /// </summary>
        [IsVisibleInDynamoLibrary(false)]
        public DelegateCommand SelectionCommand { get; set; }

        #endregion

        #region constructor

        /// <summary>
        /// The constructor for a NodeModel is used to create
        /// the input and output ports and specify the argument
        /// lacing.
        /// </summary>
        public SelectFusionEntityNodeModel()
        {
            // When you create a UI node, you need to do the
            // work of setting up the ports yourself. To do this,
            // you can populate the InPortData and the OutPortData
            // collections with PortData objects describing your ports.
            //InPortData.Add(new PortData("something", Resources.CustomNodeModePortDataInputToolTip));

            // Nodes can have an arbitrary number of inputs and outputs.
            // If you want more ports, just create more PortData objects.
            OutPortData.Add(new PortData("FusionEntities", "FusionEntity[]"));

            // This call is required to ensure that your ports are
            // properly created.
            RegisterAllPorts();

            // The arugment lacing is the way in which Dynamo handles
            // inputs of lists. If you don't want your node to
            // support argument lacing, you can set this to LacingStrategy.Disabled.
            ArgumentLacing = LacingStrategy.CrossProduct;

            // We create a DelegateCommand object which will be 
            // bound to our button in our custom UI. Clicking the button 
            // will call the SelectFusionEntities method.
            SelectionCommand = new DelegateCommand(SelectFusionEntities, CanExecuteSelection);

        }

        #endregion

        #region command methods

        private static bool CanExecuteSelection(object obj)
        {
            //
            // Perform checks before invoking selection process; for now just return true;
            //
            return true;
        }

        private static void SelectFusionEntities(object obj)
        {
            //
            // Perform selection by calling Notify();
            //
        }

        #endregion
    }
    /// <summary>
    ///     View customizer for CustomNodeModel Node Model.
    /// </summary>
    public class SelectFusionEntityNodeViewCustomization : INodeViewCustomization<SelectFusionEntityNodeModel>
    {
        /// <summary>
        /// At run-time, this method is called during the node 
        /// creation. Here you can create custom UI elements and
        /// add them to the node view, but we recommend designing
        /// your UI declaratively using xaml, and binding it to
        /// properties on this node as the DataContext.
        /// </summary>
        /// <param name="model">The NodeModel representing the node's core logic.</param>
        /// <param name="nodeView">The NodeView representing the node in the graph.</param>
        public void CustomizeView(SelectFusionEntityNodeModel model, NodeView nodeView)
        {
            // The view variable is a reference to the node's view.
            // In the middle of the node is a grid called the InputGrid.
            // We reccommend putting your custom UI in this grid, as it has
            // been designed for this purpose.

            // Create an instance of our custom UI class (defined in xaml),
            // and put it into the input grid.
            var selectionControl = new SelectionControl();
            nodeView.inputGrid.Children.Add(selectionControl);

            // Set the data context for our control to be this class.
            // Properties in this class which are data bound will raise 
            // property change notifications which will update the UI.
            selectionControl.DataContext = model;
        }

        /// <summary>
        /// Here you can do any cleanup you require if you've assigned callbacks for particular 
        /// UI events on your node.
        /// </summary>
        public void Dispose() { }
    }
}
