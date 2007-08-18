// .NAME vtkSlicerApplication
// .SECTION Description
// Contains slicer's style, application and collection of associated guis.

#ifndef __vtkSlicerApplication_h
#define __vtkSlicerApplication_h

#include "vtkSlicerBaseGUIWin32Header.h"
#include "vtkKWApplication.h"
#include "vtkKWRegistryHelper.h" // really could have been avoided :(
#include "itkMutexLock.h"

#include "vtkSlicerApplicationGUI.h"

#ifdef USE_PYTHON
#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif
#endif


class vtkSlicerModuleGUI;
class vtkSlicerGUILayout;
class vtkSlicerTheme;
class vtkSlicerGUICollection;
//BTX
class DisplayMessageQueue;
//ETX

// Description:
// Contains slicer's style, application and collection of associated guis.
//
class VTK_SLICER_BASE_GUI_EXPORT vtkSlicerApplication : public vtkKWApplication
{
 public:
    vtkTypeRevisionMacro ( vtkSlicerApplication, vtkKWApplication );

    // Description:
    // This is a singleton pattern New.  There will only be ONE
    // reference to a vtkSlicerApplication object per process.  Clients that
    // call this must call Delete on the object so that the reference
    // counting will work.   The single instance will be unreferenced when
    // the program exits.
    static vtkSlicerApplication* New();

    // Description:
    // Get the singleton
    static vtkSlicerApplication* GetInstance();

    // Description:
    // Get the layout, theme, GUI collection and main application GUI 
    vtkGetObjectMacro ( MainLayout, vtkSlicerGUILayout );
    vtkGetObjectMacro ( SlicerTheme, vtkSlicerTheme );
    vtkGetObjectMacro ( ModuleGUICollection, vtkSlicerGUICollection );
    vtkGetObjectMacro ( ApplicationGUI, vtkSlicerApplicationGUI );
    vtkSetObjectMacro ( ApplicationGUI, vtkSlicerApplicationGUI );

    // Description:
    // This method collects GUIs added to Slicer.
    virtual void AddModuleGUI ( vtkSlicerModuleGUI *gui );
    virtual void RemoveModuleGUI ( vtkSlicerModuleGUI *gui );
    virtual vtkSlicerModuleGUI* GetModuleGUIByName ( const char *name );

    // Description:
    // These methods manage windows associated with the application
    virtual void CloseAllWindows ( ) ;
    
    // Description:
    // Sets application behavior.
    virtual void ConfigureApplication ( );
    virtual int StartApplication ( );

    // Description:
    // installs rules to specify look & feel.
    virtual void InstallTheme ( vtkKWTheme *theme );

  // Description:
  // methods to access registry from wrapped languages
  int HasRegistry(const char *key);
  void RequestRegistry(const char *key);
  const char *GetRegistryHolder();
  void SetRegistry(const char *key, char *value);

  // Description:
  // Save/Retrieve the application settings to/from registry.
  // Do not call that method before the application name is known and the
  // proper registry level set (if any).
  virtual void RestoreApplicationSettingsFromRegistry();
  virtual void SaveApplicationSettingsToRegistry();

  // Description:
  // Some constants
  //BTX
  static const char *ConfirmDeleteRegKey;
  static const char *ModulePathRegKey;
  static const char *TemporaryDirectoryRegKey;
  static const char *HomeModuleRegKey;
  static const char *LoadCommandLineModulesRegKey;
  static const char *EnableDaemonRegKey;
  static const char *ApplicationFontFamilyRegKey;
  static const char *ApplicationFontSizeRegKey;
  static const char *ApplicationWindowWidthRegKey;
  static const char *ApplicationWindowHeightRegKey;
  static const char *ApplicationSlicesFrameHeightRegKey;
  static const char *ApplicationLayoutTypeRegKey;
  //ETX


  // Description:
  // Set/Get the application font family
  void SetApplicationFontFamily ( const char *family);
  const char *GetApplicationFontFamily ( ) const;

  // Description:
  // Set/Get the application font size
  void SetApplicationFontSize ( const char *size );
  const char *GetApplicationFontSize ( ) const;

  // Descrition:
  // Set/Get the confirm delete flag
  void SetConfirmDelete(const char* state);
  const char* GetConfirmDelete() const;
  
  // Description:
  // Set/Get the search path for modules.
  void SetModulePath(const char *path);
  const char* GetModulePath() const;

  // Description:
  // Set/Get a user's home module.
  void SetHomeModule (const char *name);
  const char *GetHomeModule() const;
  
  // Description:
  // Set/Get a directory for temporary file storage
  void SetTemporaryDirectory(const char *path);
  const char* GetTemporaryDirectory() const;

  // Description:
  // Set/Get the application window size
  // for saving in the registry.
  void SetApplicationWindowSize (int width, int height );
  vtkGetMacro (ApplicationWindowWidth, int);
  vtkSetMacro (ApplicationWindowWidth, int);
  vtkGetMacro (ApplicationWindowHeight, int);
  vtkSetMacro (ApplicationWindowHeight, int);

  // Description:
  // Saves the application window size for registry setting
  void SaveApplicationWindowConfiguration ( );

  // Description:
  // Set/Get the application layout for saving in
  // the registry.
  vtkGetMacro (ApplicationSlicesFrameHeight, int );
  vtkSetMacro (ApplicationSlicesFrameHeight, int );
  vtkGetMacro (ApplicationLayoutType, int );
  vtkSetMacro (ApplicationLayoutType, int );

  // Description:
  // Set/Get if command line modules should be loaded
  vtkSetMacro(LoadCommandLineModules, int);
  vtkGetMacro(LoadCommandLineModules, int);
  vtkBooleanMacro(LoadCommandLineModules, int);

  // Description:
  // Set/Get if the slicer daemon should be loaded
  vtkSetMacro(EnableDaemon, int);
  vtkGetMacro(EnableDaemon, int);
  vtkBooleanMacro(EnableDaemon, int);

  // Description:
  // Evaluate a string as a tcl expression
  const char *Evaluate(const char *expression);

  // Description:
  // Request that a message be placed in the logger
  // widget. RequestDisplayMessage() allows records to be added to the
  // logger widget from a separate thread. First argument is the type
  // of message ("Error", "Warning", "Information", "Debug").  Second
  // argument is the message to display.
  bool RequestDisplayMessage( const char *type, const char* message );
  
  // Description:
  // Process a request to place a message on the log widget. This
  // method is called in the main thread of the application because
  // calls to the log widget can cause an update to the GUI. (Method
  // needs to be public to fit in the event callback chain.)
  void ProcessDisplayMessage();

  // Description:
  // Put a message in the logger widget.  These methods actually
  // schedule the message display so that only the application
  // associated with the user interface attempts to display the
  // message. THese methods delegate to RequestDisplayMessage().
  virtual void WarningMessage(const char* message);
  virtual void ErrorMessage(const char* message);
  virtual void DebugMessage(const char* message);
  virtual void InformationMessage(const char* message);
  
  // Description:
  // Override the KWWidgets default behavior of setting the 'transient'
  // flag on the interactor, which prevents it from being hidden
  // by the main application window
  virtual void DisplayTclInteractor(vtkKWTopLevel *master);
  virtual void DisplayLogDialog(vtkKWTopLevel *master);

#ifdef USE_PYTHON
//BTX
  virtual void InitializePython( PyObject* mod, PyObject* dict )
  { PythonModule = mod; 
    PythonDictionary = dict; };
  vtkGetMacro(PythonModule, PyObject*);
  vtkGetMacro(PythonDictionary, PyObject*);
//ETX
#endif
  // Description:
  // Add additional copyright messages
  virtual void AddAboutCopyrights(ostream &);

  // Description:
  // Control the state of the splash screen
  vtkSetMacro (UseSplashScreen, int);
  vtkGetMacro (UseSplashScreen, int);

  // Description:
  // Control the state of the splash screen
  void SplashMessage (const char * message);


 protected:
  vtkSlicerApplication ( );
  virtual ~vtkSlicerApplication ( );

  vtkSlicerGUILayout *MainLayout;
  vtkSlicerTheme *SlicerTheme;
  
  // Description:
  // The main application GUI.
  vtkSlicerApplicationGUI *ApplicationGUI;
  
  // Description:
  // Collections of GUIs
  vtkSlicerGUICollection *ModuleGUICollection;

  
  char ConfirmDelete[vtkKWRegistryHelper::RegistryKeyValueSizeMax];
  char ModulePath[vtkKWRegistryHelper::RegistryKeyValueSizeMax];
  char TemporaryDirectory[vtkKWRegistryHelper::RegistryKeyValueSizeMax];
  char HomeModule [ vtkKWRegistryHelper::RegistryKeyValueSizeMax];
  char ApplicationFontSize [vtkKWRegistryHelper::RegistryKeyValueSizeMax];
  char ApplicationFontFamily [vtkKWRegistryHelper::RegistryKeyValueSizeMax];
  
  int ApplicationWindowWidth;
  int ApplicationWindowHeight;
  int ApplicationSlicesFrameHeight;
  int ApplicationLayoutType;

  char RegistryHolder [vtkKWRegistryHelper::RegistryKeyValueSizeMax];

  int LoadCommandLineModules;
  int EnableDaemon;
#ifdef USE_PYTHON
  PyObject* PythonModule;
  PyObject* PythonDictionary;
#endif

private:
  vtkSlicerApplication ( const vtkSlicerApplication& ); // Not implemented.
  void operator = ( const vtkSlicerApplication& ); //Not implemented.
    
  //BTX
  itk::MutexLock::Pointer DisplayMessageQueueActiveLock;
  itk::MutexLock::Pointer DisplayMessageQueueLock;
  //ETX
  bool DisplayMessageQueueActive;

  DisplayMessageQueue* InternalDisplayMessageQueue;
  
  static vtkSlicerApplication* Instance;

  int UseSplashScreen;
}; 

#endif
