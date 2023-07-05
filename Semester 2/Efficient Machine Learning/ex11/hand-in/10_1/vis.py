from vtk import vtkXMLRectilinearGridReader, vtkDataSetMapper, vtkActor, vtkRenderer, vtkRenderWindow, vtkRenderWindowInteractor, vtkLookupTable, vtkPlane, vtkCutter, vtkNamedColors, vtkRectilinearGrid

## READING DATA ##
reader = vtkXMLRectilinearGridReader()
reader.SetFileName('vtk/data_train.vtr')
reader.Update()
seismic = reader.GetOutput()

labelreader = vtkXMLRectilinearGridReader()
labelreader.SetFileName('vtk/labels_train.vtr')
labelreader.Update()
labels = labelreader.GetOutput()
labeldata = vtkRectilinearGrid()
labeldata.DeepCopy(seismic)
labeldata.GetCellData().SetScalars(labels.GetCellData().GetScalars())

## GENERATING PLANES TO SLICE WITH ##

plane = vtkPlane()
plane.SetOrigin(0, 0, 0)
plane.SetNormal(1, 0, 0) # (1,0,0) for yz slice, (0,1,0) for xz slice, (0,0,1) for xy slice

#plane2 = vtkPlane()
#plane2.SetOrigin(0, 0, 0)
#plane2.SetNormal(1, 0, 0)


## SLICING THE DATA ##

cutter = vtkCutter()
cutter.SetCutFunction(plane)
cutter.SetInputConnection(reader.GetOutputPort())
cutter.GenerateValues(1, 1, 1)
labelcutter = vtkCutter()
labelcutter.SetCutFunction(plane)
labelcutter.SetInputData(labeldata)
labelcutter.GenerateValues(1, 1, 1)

#cutter2 = vtkCutter()
#cutter2.SetCutFunction(plane2)
#cutter2.SetInputConnection(reader.GetOutputPort())
#cutter2.GenerateValues(1, 1, 1)


## GENERATING LOOK UP TABLES ##

lut = vtkLookupTable()
lut.SetHueRange(0, 0)
lut.SetSaturationRange(0, 0)
lut.SetValueRange(0, 1)
lut.Build()

labellut = vtkLookupTable() # I could not get this to have more contrast, so the color maps are kind of bad
labellut.SetHueRange(0, 1)
labellut.SetSaturationRange(1, 1)
labellut.SetValueRange(1, 1)
labellut.SetAlphaRange(0.5, 0.5)
labellut.Build()


## MAPPING THE DATA ##

cutterMapper = vtkDataSetMapper()
cutterMapper.SetInputConnection(cutter.GetOutputPort())
cutterMapper.SetScalarRange(seismic.GetScalarRange())
cutterMapper.SetLookupTable(lut)

labelMapper = vtkDataSetMapper()
labelMapper.SetInputConnection(labelcutter.GetOutputPort())
labelMapper.SetScalarRange(labels.GetScalarRange())
labelMapper.SetLookupTable(labellut)

#cutter2Mapper = vtkDataSetMapper()
#cutter2Mapper.SetInputConnection(cutter2.GetOutputPort())
#cutter2Mapper.SetScalarRange(seismic.GetScalarRange())
#cutter2Mapper.SetLookupTable(lut)


## GENERATING ACTORS AND RENDERING ##

planeActor = vtkActor()
planeActor.SetMapper(cutterMapper)
planeActor.AddPosition([0, -100, 0])

labelActor = vtkActor()
labelActor.SetMapper(labelMapper)
labelActor.AddPosition([0, -101, 0])

#plane2Actor = vtkActor()
#plane2Actor.GetProperty().SetColor(vtkNamedColors().GetColor3d('Blue'))
#plane2Actor.GetProperty().SetLineWidth(2)
#plane2Actor.GetProperty().SetAmbient(1.0)
#plane2Actor.GetProperty().SetDiffuse(0.0)
#plane2Actor.SetMapper(cutter2Mapper)
#plane2Actor.AddPosition([-100, 0, 0])

renderer = vtkRenderer()
renderer.AddActor(planeActor)
renderer.AddActor(labelActor)
#renderer.AddActor(plane2Actor)
renderer.SetBackground(1, 1, 1) # Set background to white

renderer_window = vtkRenderWindow()
renderer_window.AddRenderer(renderer)

interactor = vtkRenderWindowInteractor()
interactor.SetRenderWindow(renderer_window)
interactor.Initialize()
interactor.Start()