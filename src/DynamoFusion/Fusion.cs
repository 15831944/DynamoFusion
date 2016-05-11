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
        public static IEnumerable<Geometry> SelectEntity()
        {
            var geometries = new List<Geometry>();
            var entities = FusionEntity.getSelectedEntities();
            foreach (var entity in entities)
            {
                geometries.AddRange(ToDynamoGeometry(entity));
            }
            return geometries;
        }

        public static IEnumerable<FusionEntity> ImportGeometry(IEnumerable<Geometry> geometries)
        {
            var entities = new List<FusionEntity>();
            var id = 1;
            foreach (var geometry in geometries)
            {
                var curve = geometry as Curve;
                if (curve != null)
                {
                    if (!IDList.Contains(id))
                    { 
                        IDList.Add(id);
                    }
                    entities.Add(ToFusionCurve(curve, id));
                    id++;
                }
            }
            return entities;
            /*
            var geometries = curves.ToArray();
            var circle = geometries[0] as Circle;
            FusionCurve entity = null;

            if (circle != null)
            {
                if (IDList.Count() == 0)
                {
                    IDList.Add(1);
                    entity = ToFusionCurve(circle, 0);
                }
                else
                {
                    var i = 0;
                        
                    while (i < IDList.Count())
                    {
                        if (IDList.Contains(IDList[i]))
                        {
                            entity = ToFusionCurve(circle, IDList[i]);
                            return entity;
                        }
                            
                        i++;
                    }
                    if(i == IDList.Count())
                    {
                        //var idToAdd = IDList.Count() + 1;
                        IDList.Add(IDList[i-1]+1);
                        entity = ToFusionCurve(circle, IDList[i-1] + 1);
                    }
                }
            }
            
            return entity;
            */
        }

        private static FusionCurve ToFusionCurve(Curve curve, int indentifier)
        {
            var cv = curve as Circle;
            if(cv != null)
            {
                var point = cv.CenterPoint;
                FusionCurve obj = FusionCurve.createCircle(point.X, point.Y, point.Z, cv.Radius, indentifier);
                return obj;
            }
            return null;
        }
        private static IEnumerable<Geometry> ToDynamoGeometry(FusionEntity entity)
        {
            var geometries = new List<Geometry>();
            var fusionSolid = entity as FusionSolid;
            if (fusionSolid != null)
            {
                geometries.AddRange(fusionSolid.Decompose());
            }
            return geometries;
        }
    }
}
