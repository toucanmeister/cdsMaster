from vtk import vtkXMLRectilinearGridReader, vtkDataSetMapper, vtkActor, vtkRenderer, vtkRenderWindow, vtkRenderWindowInteractor, vtkLookupTable, vtkPlane, vtkCutter, vtkNamedColors

reader = vtkXMLRectilinearGridReader()
reader.SetFileName('vtk/data_train.vtr')
reader.Update()
seismic = reader.GetOutput()

labelreader = vtkXMLRectilinearGridReader()
labelreader.SetFileName('vtk/labels_train.vtr')
labelreader.Update()
labels = labelreader.GetOutput()

label_scalars = labels.GetCellData().GetScalars()
seismic.GetCellData().SetScalars(label_scalars)

lut = vtkLookupTable()
lut.SetHueRange(0, 1)
lut.SetSaturationRange(1, 1)
lut.SetValueRange(1, 1)
lut.SetAlphaRange(0.5, 0.5)
lut.Build()

mapper = vtkDataSetMapper()
mapper.SetInputData(seismic)
mapper.SetScalarRange(labels.GetScalarRange())
mapper.SetLookupTable(lut)

actor = vtkActor()
actor.SetMapper(mapper)

renderer = vtkRenderer()
renderer.AddActor(actor)
renderer.SetBackground(1, 1, 1) # Set background to white

renderer_window = vtkRenderWindow()
renderer_window.AddRenderer(renderer)

interactor = vtkRenderWindowInteractor()
interactor.SetRenderWindow(renderer_window)
interactor.Initialize()
interactor.Start()