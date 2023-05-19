import vtk # the visualization toolkit

# Data structures for point field
gridSize = 16
points = vtk.vtkPoints()

# Create point geometry (the coordinates)
for i in range(gridSize):
    for j in range(gridSize):
        # calculate coordinates
        x = i / (gridSize - 1.0) * 3.0 - 1.0
        y = j / (gridSize - 1.0) * 3.0 - 2.0
        z = x * x**2 / 3.0 + y * y**2 / 3.0 - x * x / 2.0 + y * y / 2.0
        
        # insert the point (geometry)
        points.InsertNextPoint(x,y,z)

## 2a)
polydata = vtk.vtkPolyData()
polydata.SetPoints(points)

## 2b)
vertexFilter = vtk.vtkVertexGlyphFilter()
vertexFilter.SetInputData(polydata)
vertexFilter.Update()

## 2c)
mapper = vtk.vtkPolyDataMapper()
mapper.SetInputConnection(vertexFilter.GetOutputPort())

actor = vtk.vtkActor()
actor.SetMapper(mapper)
actor.GetProperty().SetPointSize(8)
actor.GetProperty().SetRenderPointsAsSpheres(True)
actor.GetProperty().SetColor(1.0, 0.0, 0.0)

renderer = vtk.vtkRenderer()
renderer.AddActor(actor)
renderer.SetBackground(0.8, 0.8, 0.8) # light gray color

renderWindow = vtk.vtkRenderWindow()
renderWindow.SetSize(300, 300) # size in pixel
renderWindow.AddRenderer(renderer)

renderWindowInteractor = vtk.vtkRenderWindowInteractor()
renderWindowInteractor.SetRenderWindow(renderWindow)
renderWindowInteractor.SetInteractorStyle(vtk.vtkInteractorStyleTrackballCamera())


## 3a)
delaunayFilter = vtk.vtkDelaunay2D()
delaunayFilter.SetInputData(polydata)
delaunayFilter.Update()

## 3b)
meshMapper = vtk.vtkPolyDataMapper()
meshMapper.SetInputConnection(delaunayFilter.GetOutputPort())

meshActor = vtk.vtkActor()
meshActor.SetMapper(meshMapper)
meshActor.GetProperty().SetEdgeVisibility(True)

renderer.AddActor(meshActor)

renderWindowInteractor.Start()