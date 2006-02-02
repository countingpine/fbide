#ifndef _CLASS_PLUGIN_SERVER_H_
#define _CLASS_PLUGIN_SERVER_H_

namespace ID_Error {
    enum Plugin {
        PLUGIN_OK = 0,
        PLUGIN_FILE_NOT_FOUND,
        PLUGIN_INVALID_FILE_NAME,
        PLUGIN_INVALID_FILE_TYPE,
        PLUGIN_INVALID_FILE_VERSION,
        PLUGIN_UNKNOWN_LOAD_ERROR,
        PLUGIN_ALREADY_LOADED,
        PLUGIN_INIT_ERROR,
    };
}

class classPlugin;
class wxDynamicLibrary;

class classPluginServer {

    private:
        
        /**
         * @brief is a helper class for classPluginServer
         * and holds info about loaded plugins.
         * this class is used in the array created by WX_DEFINE_ARRAY
         * faciliyu
         */
        class classPluginList {
            public:
                
                /**
                 * @brief constructor
                 * @param ID - ID of the loaded plugin
                 * @param objDll - object dll accociated
                 * @param objPlugin - plugin itself
                 * @param objFileName - filename of the plugin
                 */
                classPluginList( int ID, wxDynamicLibrary * objDll, 
                    classPlugin * objPlugin, wxFileName objFileName ) : 
                        m_ID( ID ), m_objDll( objDll ), 
                        m_objPlugin( objPlugin ), m_objFileName( objFileName ) 
                { }
                
                
                /**
                 * @var m_ID - ID of the plugin
                 * @var m_objDll - a dll handler accociated with this plugin
                 * @var m_objPlugin - a pointer plugin itself
                 * @var m_objFileName - filename of the plugin
                 */
                int                   m_ID;
                wxDynamicLibrary    * m_objDll;
                classPlugin         * m_objPlugin;
                wxFileName            m_objFileName;
        };

        WX_DEFINE_ARRAY( classPluginList *,  arrayOfPlugins );
        
        
        /**
         * @var m_ParentApp - a pointer to wxApp instance
         * @var m_ParentFrame - a pointer to the main frame accociated with the 
         *                      program
         * @var m_arrPlugins - array of loaded plugins
         * @var m_intPluginID - this incerements every time plugin is loaded
         *                      so every plugin receaves an unique ID
         */
        wxApp               * m_ParentApp;
        wxFrame             * m_ParentFrame;
        arrayOfPlugins      m_arrPlugins;
        int                 m_intPluginID;
        ID_Error::Plugin    m_ErrorCode;
        
        
        /**
         * @breaf Sets the error code
         * @param the error code
         */
        void SetError( const ID_Error::Plugin errorCode ) { 
            m_ErrorCode = errorCode;
        }
    
    public:
        
        /**
         * @brief Is default constructor.
         * @param wxApp * - pointer of the wxApp instance
         * @param wxFrame * - pointer of the wxFrame instance
         */
        classPluginServer( wxApp * objApp, wxFrame * objFrame ) : 
            m_ParentApp( objApp ), m_ParentFrame( objFrame )
        { }
        
        
        /**
         * @brief Is destructor
         * Cleans up and unloads all loaded plugins
         */
        ~classPluginServer ();
        
        /**
         * @breaf Returns the number of loaded plugins
         * @return const int - number of plugins
         */
        const int GetPluginCount() const { return m_arrPlugins.GetCount(); }
        
        
        /**
         * @brief This function loads a new plugin.
         * @param wxFileName objFile - A file to load
         * @return int - plugin ID number >= 0 or -1 on failure.
         */
        int LoadPlugin ( wxFileName objFile );
        
        
        /**
         * @brief Unloads the loaded plugin
         * @param int pluginID - ID of the plugin
         * @return none
         */
        void UnloadPlugin ( const int pluginID );
        
        
        /**
         * @brief Checks if the plugin with the given ID is loaded ot not
         * @param int pluginID - Id of the plugin we want to check
         * @return bool - true or false
         */
        bool IsLoaded ( const int pluginID );
        
        
        /**
         * @brief Checks if the plugin with the given filename is loaded or not
         * @param wxFileName - filename of the plugin
         * @return int -plugin ID or -1 on failure
         */
        int GetPluginId ( wxFileName objFile );
        
        
        /**
         * @brief Gets the plugin by filename or NULL on failure
         * @param wxFileName objFile filename of the plugin
         * @return classPlugin *
         */
        const classPlugin * GetPlugin ( wxFileName objFile ) {
            int pluginID = GetPluginId ( objFile );
            if ( pluginID != -1 ) return GetPlugin( pluginID );
            else return NULL;
        }
        
        
        /**
         * @brief Gets the pointer of the specified plugin
         * @param int pluginID - the ID of the plugin to retreave
         * @return classPlugin *
         */
        const classPlugin * GetPlugin ( int pluginID ) {
            return m_arrPlugins.Item( pluginID )->m_objPlugin;
        }
        
        
        
        /**
         * @brief If there was an regarding dll's then this is a function
         *        that returns the error code.
         * @return errorPlugin
         */
        const ID_Error::Plugin GetError () const { return m_ErrorCode; }
        
};

#endif