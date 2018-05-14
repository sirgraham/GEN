/*------------------------------------------------------------------------------------------
//  GRPMATERIAL.H
*/
/**
// \class
//
//  Graphics Material class
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 07/05/2014 16:00:48
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPMATERIAL_H_
#define _GRPMATERIAL_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"

#include "GRPMatrix.h"
#include "GRPOpenGL.h"
#include "GRPColor.h"
#include "GRPTexture.h"
#include "GRPInterfaceAnimable.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/



/*---- CLASS -----------------------------------------------------------------------------*/

class GRPLIGHT;

class GRPMATERIALATTRIBUTES
{
  public:
                                GRPMATERIALATTRIBUTES       ()                        { Clean();                                                    }
    virtual                    ~GRPMATERIALATTRIBUTES       ()                        { Clean();                                                    }

    GLFLOAT                     GetGlossiness               ()                        { return glossiness;                                          }
    void                        SetGlossiness               (GLFLOAT glossiness)      { this->glossiness = glossiness;            isupdated=true;   }

    GLFLOAT                     GetShininess                ()                        { return shininess;                                           }
    void                        SetShininess                (GLFLOAT shininess)       { this->shininess = shininess;              isupdated=true;   }

    GLFLOAT                     GetShininessFactor          ()                        { return shininessFactor;                                     }
    void                        SetShininessFactor          (GLFLOAT shininess)       { this->shininessFactor = shininess;        isupdated=true;   }

    GRPCOLOR*                   GetSpecular                 ()                        { return &specular;                                           }
    void                        SetSpecular                 (GRPCOLOR specular)       { this->specular = specular;                isupdated=true;   }

    GLFLOAT                     GetSpecularFactor           ()                        { return specularFactor;                                      }
    void                        SetSpecularFactor           (GLFLOAT specular)        { this->specularFactor = specular;          isupdated=true;   }

    GLFLOAT                     GetTransparency             ()                        { return transparencyfactor;                                  }
    void                        SetTransparency             (GLFLOAT transparency)    { this->transparencyfactor = transparency;  isupdated=true;   }

    void                        SetBumpFactor               (GLFLOAT bumpfactor)      { this->bumpfactor = bumpfactor;            isupdated=true;   }
    GLFLOAT                     GetBumpFactor               ()                        { return this->bumpfactor;                                    }

    GRPCOLOR*                   GetAmbient                  ()                        { return &ambient;                                            }
    void                        SetAmbient                  (GRPCOLOR ambient)        { this->ambient = ambient;                  isupdated=true;   }

    GRPCOLOR*                   GetDiffuse                  ()                        { return &diffuse;                                            }
    void                        SetDiffuse                  (GRPCOLOR p)              { this->diffuse=p;                          isupdated=true;   }

    GRPCOLOR*                   GetEmission                 ()                        { return &emission;                                           }
    void                        SetEmission                 (GRPCOLOR p)              { this->emission=p;                         isupdated=true;   }

    GLFLOAT                     GetDissolve                 ()                        { return dissolve;                                            }
    void                        SetDissolve                 (GLFLOAT p)               { this->dissolve=p;                         isupdated=true;   }

    bool                        IsIlluminated               ()                        { return illumination;                                        }
    void                        SetIsIlluminated            (bool p)                  { this->illumination=p;                     isupdated=true;   }

    void                        SetIntensity                (float f)                 { this->intensity=f;                                          }
    GLFLOAT                     GetIntensity                ()                        { return this->intensity;                   isupdated=true;   }


    void                        UseMaterialData             ( float externalIntensity, GLINT link );
    void                        UseMaterialData             ( float externalIntensity, GRPCOLOR* diffusecolor, GLINT link );
    void                        UseMaterialData             ( float externalIntensity, GRPLIGHT* light ,GLINT link );
    void                        UpdateMaterialMatrix        ();

    bool                        isupdated;

  private:

    void                        Clean                       ()
                                {
                                  glossiness          = 0.0f;
                                  shininess           = 0.0f;
                                  shininessFactor     = 1.0f;
                                  specularFactor      = 1.0f;
                                  dissolve            = 0.0f;

                                  transparencyfactor  = 1.0f;
                                  intensity           = 1.00f;
                                  bumpfactor          = 1.0f;
                                  isupdated           = false;

                                  materialMatrix.LoadIdentity();
                                  diffuse.Set         (1.0f, 1.0f, 1.0f, 1.0f);
                                  specular.Set        (1.0f, 1.0f, 1.0f, 1.0f);
                                  ambient.Set         (1.0f, 1.0f, 1.0f, 1.0f);
                                  emission.Set        (0.0f, 0.0f, 0.0f, 0.0f);
                                }

    bool                        illumination;

    GLFLOAT                     glossiness;
    GLFLOAT                     shininess;
    GLFLOAT                     shininessFactor;

    GLFLOAT                     dissolve;

    GRPCOLOR                    specular;
    GLFLOAT                     specularFactor;

    GRPCOLOR                    ambient;
    GRPCOLOR                    diffuse;
    GRPCOLOR                    emission;

    GLFLOAT                     intensity;
    GLFLOAT                     transparencyfactor;
    GLFLOAT                     bumpfactor;

    GRPMATRIX                   materialMatrix;


};




class GRPMATERIAL : public GRPINTERFACEANIMABLE
{
  public:
                                      GRPMATERIAL                 ();
          virtual                    ~GRPMATERIAL                 ();

          bool                        IsUsable                    ()                              { return isusable;                      }

          void                        AddLayer                    (GRPTEXTURE* texture)           { layers.Add(layers.GetSize(),texture); }
          void                        AddLayer                    (XDWORD id,GRPTEXTURE* texture) { layers.Add(id,              texture); }
          void                        SetLayer                    (XDWORD id,GRPTEXTURE* texture)
          {
            if (!layers.Set(id,texture))
              layers.Add(id,texture);
          }

          bool                        Select                      ();

          GRPTEXTURE*                 GetLayer                    (XDWORD e)                      { return layers.Get(e);                 }
          GRPMATERIALATTRIBUTES*      GetAttributes               ()                              { return &attributes;                   }
          void                        SetAttributes               (GRPMATERIALATTRIBUTES p)       { this->attributes=p;                   }
          XMAP<XDWORD,GRPTEXTURE*>*   GetLayers                   ()                              { return &layers;                       }

          void                        SetShadingModelName         (XSTRING s)                     { this->ShaderModelName=s;              }
          XSTRING*                    GetShadingModelName         ()                              { return &this->ShaderModelName;        }

virtual   bool                        SetValue                    (GRPANIMATIONCURVETYPE type,GRPANIMATIONCURVEAXIS subtype, double value,double base);

  private:

          void                        Clean                       ()
                                      {
                                        isusable        = false;
                                        ShaderModelName =__L("lambert");
                                        layers.DeleteAll();
                                          ///---------------------- default attributes

                                        GetAttributes()->GetDiffuse()->Set(1.0f,1.0f,1.0f,1.0f);
                                        GetAttributes()->GetAmbient()->Set(1.0f,1.0f,1.0f,1.0f);
                                        GetAttributes()->GetSpecular()->Set(1.0f,1.0f,1.0f,1.0f);

                                        GetAttributes()->SetIntensity(1.0f);
                                        GetAttributes()->isupdated=true;
                                        GetAttributes()->UpdateMaterialMatrix();


                                      }

          bool                        isusable;

          XMAP<XDWORD,GRPTEXTURE*>    layers;
          GRPMATERIALATTRIBUTES       attributes;
          XSTRING                     ShaderModelName;


};







/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

