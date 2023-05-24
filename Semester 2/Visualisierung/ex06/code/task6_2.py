import vtk
import numpy as np

# Returns vertex-ids in a mesh, which share
# an edge with the vertex at a specified seed id,
# i.e., finds the one-ring neighborhood
def getConnectedVertices(mesh, seed_id):
    # use a set to avoid multiple references to the same point
    connected_vertex_ids = set()

    # retrieve all cells containing the seed point
    # (cells are commonly polygons, edges and vertices)
    cellIdList = vtk.vtkIdList()
    mesh.GetPointCells(seed_id, cellIdList)

    # iterate these cells
    for i in range(cellIdList.GetNumberOfIds()):
        cell = mesh.GetCell(cellIdList.GetId(i))
        if cell.GetNumberOfEdges() <= 0:
            # cell has no edges, so it's a line or a point
            line = vtk.vtkLine.SafeDownCast(cell)
            if not line: # it's a point
                continue
            else: # it's a line
                p0 = line.GetPointId(0)
                p1 = line.GetPointId(1)
                if p0 == seed_id:
                    connected_vertex_ids.add(p1)
                else:
                    connected_vertex_ids.add(p0)
            continue

        # if we get to here, the cell is a polygon, so extract its border edges
        for e in range(cell.GetNumberOfEdges()):
            edge = cell.GetEdge(e)
            pointIdList = edge.GetPointIds()

            if(pointIdList.GetId(0) == seed_id or pointIdList.GetId(1) == seed_id):
                if pointIdList.GetId(0) == seed_id:
                    connected_vertex_ids.add(pointIdList.GetId(1))
                else:
                    connected_vertex_ids.add(pointIdList.GetId(0))
    return connected_vertex_ids

# Create a sphere
sphere = vtk.vtkSphereSource()
sphere.SetPhiResolution(12)
sphere.SetThetaResolution(12)
sphere.Update()

# retrieve the mesh
mesh = sphere.GetOutput()

# apply random noise on top of sphere points
np.random.seed(19640804)
for i in range(mesh.GetNumberOfPoints()):
    displacement = np.random.rand(3) * 0.09
    p = np.array(mesh.GetPoint(i))
    p += displacement
    mesh.GetPoints().SetPoint(i, p)

# copy the mesh to create the smoothed geometries
mesh_laplace = vtk.vtkPolyData()
mesh_low_pass = vtk.vtkPolyData()
mesh_laplace.DeepCopy(mesh)
mesh_low_pass.DeepCopy(mesh)


####################
# Task 2           #
####################

## a)

def laplace_filter_iteration(mesh, parameter):
    mesh_copy = vtk.vtkPolyData()
    mesh_copy.DeepCopy(mesh)
    for p_id in range(mesh.GetNumberOfPoints()):
        neighbor_ids = getConnectedVertices(mesh, p_id)
        num_neighbors = len(neighbor_ids)
        coordinates = np.array(mesh.GetPoint(p_id))
        change = np.zeros(3)
        for neighbor_id in neighbor_ids:
            neighbor_coordinates = np.array(mesh.GetPoint(neighbor_id))
            change += (1.0/num_neighbors) * (neighbor_coordinates - coordinates)
        coordinates += parameter * change
        mesh_copy.GetPoints().SetPoint(p_id, coordinates)
    return mesh_copy

for iteration in range(5):
    mesh_laplace = laplace_filter_iteration(mesh_laplace, 0.5)

## b)

for iteration in range(10):
    mesh_low_pass = laplace_filter_iteration(mesh_low_pass, 0.5)
    mesh_low_pass = laplace_filter_iteration(mesh_low_pass, -1.02*0.5)


####################
# Scene            #
####################
mapper_orig = vtk.vtkPolyDataMapper()
mapper_orig.SetInputData(mesh)
actor_orig = vtk.vtkActor()
actor_orig.SetMapper(mapper_orig)
actor_orig.GetProperty().SetInterpolationToFlat()

mapper_laplace = vtk.vtkPolyDataMapper()
mapper_laplace.SetInputData(mesh_laplace)
actor_laplace = vtk.vtkActor()
actor_laplace.SetMapper(mapper_laplace)
actor_laplace.GetProperty().SetInterpolationToFlat()
actor_laplace.SetPosition(1.5, 0, 0)

mapper_low_pass = vtk.vtkPolyDataMapper()
mapper_low_pass.SetInputData(mesh_low_pass)
actor_low_pass = vtk.vtkActor()
actor_low_pass.SetMapper(mapper_low_pass)
actor_low_pass.GetProperty().SetInterpolationToFlat()
actor_low_pass.SetPosition(3.0, 0, 0)


####################
# Renderpipeline   #
####################
# Create the renderer, add actors
renderer = vtk.vtkRenderer()
renderer.AddActor(actor_orig)
renderer.AddActor(actor_laplace)
renderer.AddActor(actor_low_pass)
renderer.SetBackground(1.0, 1.0, 1.0)

# Create the render window
renderWindow = vtk.vtkRenderWindow()
renderWindow.SetSize(1280, 720)
renderWindow.AddRenderer(renderer)

# Create the interactor
renderWindowInteractor = vtk.vtkRenderWindowInteractor()
renderWindowInteractor.SetRenderWindow(renderWindow)
renderWindowInteractor.SetInteractorStyle(vtk.vtkInteractorStyleTrackballCamera())

# This starts the event loop.
renderWindowInteractor.Start()