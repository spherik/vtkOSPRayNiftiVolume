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
#include <vtkXMLImageDataWriter.h>

int main(int argc, char *argv[])
{
  bool useOSP = true;
  bool writeVTI = false;
  vtkNew<vtkNIFTIImageReader> reader;
  reader->SetFileName("/Volumes/2T/BrainData/mindboggle/example_mri_data/T1.nii.gz");
  reader->Update();

  vtkNIFTIImageHeader* header = reader->GetNIFTIHeader();
  std::cout << header->GetDataType() << std::endl;

  if(writeVTI)
  {
    vtkNew<vtkXMLImageDataWriter> writer;
    writer->SetFileName("output.vti");
    writer->SetInputData(reader->GetOutput());
    writer->Write();
  }

  vtkNew<vtkGPUVolumeRayCastMapper> volumeMapper;
  volumeMapper->SetAutoAdjustSampleDistances(0);
  volumeMapper->SetSampleDistance(0.5);
  volumeMapper->SetInputData(reader->GetOutput());

  vtkNew<vtkVolumeProperty> volumeProperty;
  vtkNew<vtkColorTransferFunction> ctf;
  ctf->AddRGBPoint(-32768, 0.0, 0., 0.);
  ctf->AddRGBPoint(-28414, 1.,0.8,0.4);
  ctf->AddRGBPoint(-20806, 0.92549,0.917647,0.611765);
  ctf->AddRGBPoint(-14365, 0.901961,1.,0.772549);
  ctf->AddRGBPoint(17840, 1,0.,0.);
  ctf->AddRGBPoint(32767, 1.,0,0);  //

  vtkNew<vtkPiecewiseFunction> pwf;
  pwf->AddPoint(-32768, 0.0);
  pwf->AddPoint(-30476.6, 0.0);
  pwf->AddPoint(-19068, 0.5875);
  pwf->AddPoint(-17227.7, 0.16875);
  pwf->AddPoint(-13956.1, 0.24375);
  pwf->AddPoint(3833.45, 0.775);
  pwf->AddPoint(9149.86, 0.79375);
  pwf->AddPoint(32767, 0);


  volumeProperty->SetColor(ctf);
  volumeProperty->SetScalarOpacity(pwf);
  volumeProperty->SetShade(0);
  volumeProperty->SetScalarOpacityUnitDistance(1.732);

  vtkNew<vtkVolume> volume;
  volume->SetMapper(volumeMapper);
  volume->SetProperty(volumeProperty);

  // Create the renderwindow, interactor and renderer
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->SetMultiSamples(0);
  renderWindow->SetSize(1024, 768); // NPOT size
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
