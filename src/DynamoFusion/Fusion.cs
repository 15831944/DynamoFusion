using Autodesk.DesignScript.Geometry;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using FusionManagedWrapper;
using Autodesk.DesignScript.Runtime;

// This is the Dynamo-Fusion node library for Fusion specific nodes in Dynamo
namespace DynamoFusion
{
    public static class Fusion
    {
        // private static FusionCurve entity = null;

        static List<int> IDList = new List<int> { };

        [IsVisibleInDynamoLibrary(false)]
        public static IEnumerable<FusionEntity> SelectEntity()
        {
            return FusionEntity.getSelectedEntities();
        }

        public static FusionCurve ImportCurve(IEnumerable<Curve> curves)
        {
            var geometries = curves.ToArray();
            var circle = geometries[0] as Circle;
            FusionCurve entity = null;
            if (entity == null)
            {
                if (circle != null)
                {
                    if(IDList.Count()==0)
                        entity = ToFusionCurve(circle, 0);
                    else
                        entity = ToFusionCurve(circle, IDList[0]);
                }
                
            }
            
            return entity;
        }

        private static FusionCurve ToFusionCurve(Curve curve, int indentifier)
        {
            var cv = curve as Circle;
            if(cv != null)
            {
                var point = cv.CenterPoint;
                FusionCurve obj = FusionCurve.createCircle(point.X, point.Y, point.Z, cv.Radius, indentifier);
                int currID = FusionCurve.getid();
                foreach (var id in IDList) {
                    if(currID == id)
                    {
                        return obj;
                    }
                }
                IDList.Add(currID);
                return obj;
            }
            return null;
        }
    }
}
