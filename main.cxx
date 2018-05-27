#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkSphere.h>
#include <vtkSampleFunction.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVolumeProperty.h>
#include <vtkCamera.h>
#include <vtkImageShiftScale.h>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkXMLImageDataReader.h>
#include <vtkNIFTIImageReader.h>
#include <vtkOSPRayPass.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkNIFTIImageHeader.h>
#include <vtkImageImport.h>
#include <vtkInteractorStyleTrackballCamera.h>

int main(int argc, char *argv[])
{
  bool useOSP = true;

  vtkNew<vtkNIFTIImageReader> reader;
  reader->SetFileName("/Volumes/2T/BrainData/tutorial_epimap/anat/MRI/3DT1post_deface.nii");
  reader->Update();

  vtkNew<vtkGPUVolumeRayCastMapper> volumeMapper;
  volumeMapper->SetAutoAdjustSampleDistances(0);
  volumeMapper->SetSampleDistance(0.5);
  volumeMapper->SetInputData(reader->GetOutput());

  vtkNew<vtkVolumeProperty> volumeProperty;
  vtkNew<vtkColorTransferFunction> ctf;
  ctf->AddRGBPoint(37.3531, 0.2, 0.29, 1);
  ctf->AddRGBPoint(157.091, 0.87, 0.87, 0.87);
  ctf->AddRGBPoint(276.829, 0.7, 0.015, 0.15);

  vtkNew<vtkPiecewiseFunction> pwf;
  pwf->AddPoint(37.3531, 0.0);
  pwf->AddPoint(276.829, 1.0);

  volumeProperty->SetColor(ctf);
  volumeProperty->SetScalarOpacity(pwf);
  volumeProperty->SetShade(1);
  volumeProperty->SetScalarOpacityUnitDistance(1.732);

  vtkNew<vtkVolume> volume;
  volume->SetMapper(volumeMapper);
  volume->SetProperty(volumeProperty);

  // Create the renderwindow, interactor and renderer
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->SetMultiSamples(0);
  renderWindow->SetSize(401, 399); // NPOT size
  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renderWindow);
  vtkNew<vtkInteractorStyleTrackballCamera> style;
  iren->SetInteractorStyle(style);
  vtkNew<vtkRenderer> renderer;
  renderer->SetBackground(0.3, 0.3, 0.4);
  renderWindow->AddRenderer(renderer);

  renderer->AddVolume(volume);

  vtkNew<vtkOSPRayPass> osprayPass;
  if (useOSP)
  {
    renderer->SetPass(osprayPass);
  }


  renderer->ResetCamera();
  renderWindow->Render();

  iren->Start();

   return EXIT_SUCCESS;

}
