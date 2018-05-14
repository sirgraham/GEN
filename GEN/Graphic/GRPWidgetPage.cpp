/*------------------------------------------------------------------------------------------
// GRPWIDGETPAGE.CPP
*/
/**
// \class
//
// Manages Widget page
//
//  @author: Diego Martinez Ruiz de Gaona
//
//  Date of Creation : 12/12/2017 11:22:41
//  Last Modification :
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPWidgetPage.h"
#include "XFileXML.h"
#include "GRPScene.h"
#include "GRPScreen.h"
#include "INPDevice.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/
/*-------------------------------------------------------------------
//  GRPWIDGETPAGE::GRPWIDGETPAGE
*/
/**
//
//  Class Constructor GRPWIDGETPAGE
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/08/2017 11:30:57
//
*/
/*-----------------------------------------------------------------*/
GRPWIDGETPAGE::GRPWIDGETPAGE(XCHAR* toload)
{
   Clean();
   GRPSTYLESHEET*           defaultsheet            = new GRPSTYLESHEET();
                            defaultsheet->name.Set  (__L("default"));

    this->CreateWidgetFactory();
    this->CreateProcessor();


    this->processor->Init(defaultsheet);


    this->content = toload;
    this->stylesheets.Add(defaultsheet);

}

/*-------------------------------------------------------------------
//  GRPWIDGETPAGE::GRPWIDGETPAGE
*/
/**
//
//  constructor
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  22/02/2018 16:48:42
//  @return    :
//
//  @param    XCHAR* :
//  @param     GRPSTYLEPROCESSORCSS* css :
//  @param     GRPWIDGETFACTORY* factory :
//
*//*-----------------------------------------------------------------*/
GRPWIDGETPAGE::GRPWIDGETPAGE(XCHAR* toload, GRPSTYLEPROCESSORCSS* css, GRPWIDGETFACTORY* factory)
{
    Clean();
    GRPSTYLESHEET*            defaultsheet = new GRPSTYLESHEET();
    defaultsheet->name.Set(__L("default"));

    this->widgetfactory = factory;
    this->processor     = css;

    this->processor->Init(defaultsheet);


    this->content = toload;
    this->stylesheets.Add(defaultsheet);
}

/*-------------------------------------------------------------------
//  GRPWIDGETPAGE::~GRPWIDGETPAGE
*/
/**
//
//   Class Destructor GRPWIDGETPAGE
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/09/2017 10:29:54
//
*/
/*-----------------------------------------------------------------*/
GRPWIDGETPAGE::~GRPWIDGETPAGE()
{
    this->stylesheets.DeleteContentsInstanced();
    this->stylesheets.DeleteAll();
    delete(this->root);

    delete(widgetfactory);
    delete(processor);
}


/*-------------------------------------------------------------------
//   GRPWIDGETPAGE::Notify
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  14/02/2018 13:15:44
//  @return   bool :
//
//  @param    GRPMSGTYPE :
//  @param    GRPSUBJECT* :
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGETPAGE::Notify(GRPMSGTYPE msg, GRPSUBJECT* subject)
{
    GRPWIDGET* widget = dynamic_cast<GRPWIDGET*>(subject);
    if (!widget) return false;

    switch (msg)
    {
    case GRPMSG_DESTROYED:
        {

        }
        return true;
    }

    Publish(msg, widget);
    return true;
}

/*-------------------------------------------------------------------
//   GRPWIDGETPAGE::Update
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  06/02/2018 17:17:57
//  @return   bool :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGETPAGE::Update()
{
    for (XDWORD e = 0; e < inputdevices.GetSize(); e++)
    {
        INPDEVICE* device = inputdevices.Get(e);
                   device->Update();
        if (device)
        if (device->GetCursors())
        {
            float x = device->GetCursor(0)->GetX();
            float y = device->GetCursor(0)->GetY();
            y = ownerpass->GetScreen()->GetHeight() - y;

            x *= this->ownerpass->GetScale();
            y *= this->ownerpass->GetScale();

            XVECTOR<GRPWIDGET*> hits;
            if (Hit(x, y, &hits, this->root))
            for (XDWORD w = 0; w < hits.GetSize(); w++)
            {
                GRPWIDGET* widget = hits.Get(w);

                widget->Notify(UI_MOUSE_MOVE, this);

                if (device->GetButton(0)->GetState() != INPBUTTON_STATE_UP ||
                    device->GetButton(1)->GetState() != INPBUTTON_STATE_UP ||
                    device->GetButton(2)->GetState() != INPBUTTON_STATE_UP)
                    widget->Notify(UI_MOUSE_CLICK, this);

            }
            hits.DeleteAll();
        }
    }
    return true;
}

/*-------------------------------------------------------------------
//   GRPWIDGETPAGE::Add
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  06/02/2018 17:18:15
//  @return   bool :
//
//  @param    INPDEVICE* :
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGETPAGE::Add(INPDEVICE* inpdevice)
{
    this->inputdevices.Add(inpdevice);
    return true;
}

/*-------------------------------------------------------------------
//  GRPWIDGETPAGE::Add
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/08/2017 14:34:59
//
//  @return       bool :
//
//  @param        widget :
*/
/*-----------------------------------------------------------------*/
bool GRPWIDGETPAGE::Add(GRPWIDGET* widget)
{
    if (widget)
    {
        widget->page = this;
        widget->Subscribe(this);
        this->Subscribe(widget);
    }
    return true;
}

/*-------------------------------------------------------------------
//  GRPWIDGETPAGE::Add
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/08/2017 11:00:07
//
//  @return       bool :
//
//  @param        classstyle :
*/
/*-----------------------------------------------------------------*/
bool GRPWIDGETPAGE::Add(GRPWIDGETSTYLECLASS* classstyle)
{
    this->stylesheets.GetLast()->styles.Add(classstyle);
    return true;
}

/*-------------------------------------------------------------------
//  GRPWIDGETPAGE::Add
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/09/2017 18:19:36
//
//  @return       bool :
//
//  @param        sheet :
*/
/*-----------------------------------------------------------------*/
bool GRPWIDGETPAGE::Add(GRPSTYLESHEET* sheet)
{
    this->stylesheets.Add(sheet);
    return true;
}


/*-------------------------------------------------------------------
//   GRPWIDGETPAGE::ParseStyle
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  13/12/2017 13:11:57
//  @return   bool :
//
//  @param    XFILEXMLELEMENT* :
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGETPAGE::ParseStyle(XCHAR* styledata)
{
    GRPSTYLESHEET* sheet = NULL;
    if (!stylesheets.GetSize())
        this->stylesheets.Add(new GRPSTYLESHEET());

    sheet = this->stylesheets.GetLast();

    return this->processor->ParseStyle(sheet, styledata);
}


/*-------------------------------------------------------------------
//   GRPWIDGETPAGE::Set
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  05/02/2018 17:39:37
//  @return   bool :
//
//  @param    GRPWIDGET* :
//  @param    GRPWIDGETSTATE :
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGETPAGE::Set(GRPWIDGET* w, GRPWIDGETSTATE state)
{
    w->SetState(state);
    return true;
}


/*-------------------------------------------------------------------
//   GRPWIDGETPAGE::Pick
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  05/02/2018 14:04:45
//  @return   GRPWIDGET* :
//
//  @param    GLFLOAT :
//  @param    GLFLOAT :
//
*//*-----------------------------------------------------------------*/
GRPWIDGET* GRPWIDGETPAGE::Pick(GLFLOAT x, GLFLOAT y)
{
    return SubPick(x, y, this->root);
}

/*-------------------------------------------------------------------
//   GRPWIDGETPAGE::Hit
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  08/02/2018 16:49:18
//  @return   int :
//
//  @param    GLFLOAT :
//  @param    GLFLOAT :
//  @param    XVECTOR<GRPWIDGET*>*hits :
//  @param    GRPWIDGET* :
//
*//*-----------------------------------------------------------------*/
int GRPWIDGETPAGE::Hit(GLFLOAT x, GLFLOAT y, XVECTOR<GRPWIDGET*>*hits, GRPWIDGET* parent)
{
    if (parent)
    {
        if (parent->Hit(x, y))
        {
            hits->Add(parent);
        }
        for (XDWORD e = 0; e < parent->widgets.GetSize(); e++)
        {
            Hit(x, y, hits, parent->widgets.Get(e));
        }
    }
    return hits->GetSize();
}

/*-------------------------------------------------------------------
//   GRPWIDGETPAGE::SubPick
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  05/02/2018 14:05:39
//  @return   GRPWIDGET* :
//
//  @param    GLFLOAT :
//  @param    GLFLOAT :
//
*//*-----------------------------------------------------------------*/
GRPWIDGET* GRPWIDGETPAGE::SubPick(GLFLOAT x, GLFLOAT y, GRPWIDGET* widget)
{
    if (!widget)
        return NULL;

    GRPNODE* node= widget->node;

    GRPVECTOR localposition;
    localposition = node->GetWorldPosition();
    localposition.vector[0] = x - localposition.vector[0];
    localposition.vector[1] = y - localposition.vector[1];
    localposition.vector[2] = 0;

    if (localposition.vector[0]>0 && localposition.vector[0]<widget->boundingbox.width)
    if (localposition.vector[1]>0 && localposition.vector[1]<widget->boundingbox.height)
    {
        for (XDWORD e = 0; e < widget->widgets.GetSize(); e++)
        {
            GRPWIDGET* w=SubPick(x, y, widget->widgets.FastGet(e));
            if (w)
                return w;
        }
        return widget;
    }

    return NULL;
}

/*-------------------------------------------------------------------
//   GRPWIDGETPAGE::Get
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  05/02/2018 14:04:21
//  @return   GRPWIDGET* :
//
//  @param    XCHAR* :
//
*//*-----------------------------------------------------------------*/
GRPWIDGET* GRPWIDGETPAGE::Get(XCHAR* name)
{
    return FindByName(name, root);
}


/*-------------------------------------------------------------------
//   GRPWIDGETPAGE::FindByTag
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  22/02/2018 16:49:10
//  @return   GRPWIDGET* :
//
//  @param    XCHAR* :
//
*//*-----------------------------------------------------------------*/
GRPWIDGET* GRPWIDGETPAGE::FindByTag(XCHAR* tag)
{
    return FindByTag(tag,root);
}

/*-------------------------------------------------------------------
//   GRPWIDGETPAGE::FindByTag
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  22/02/2018 16:49:02
//  @return   GRPWIDGET* :
//
//  @param    XCHAR* :
//  @param    GRPWIDGET* :
//
*//*-----------------------------------------------------------------*/
GRPWIDGET* GRPWIDGETPAGE::FindByTag(XCHAR* tag, GRPWIDGET* parent)
{
    if (parent->tag.Compare(tag) == 0)
        return parent;

    for (XDWORD e = 0; e < parent->widgets.GetSize(); e++)
    {
        GRPWIDGET* w = FindByTag(tag, parent->widgets.Get(e));
        if (w)
            return w;
    }

    return NULL;
}

/*-------------------------------------------------------------------
//   GRPWIDGETPAGE::Find
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  05/02/2018 14:04:32
//  @return   GRPWIDGET* :
//
//  @param    XCHAR* :
//  @param    GRPWIDGET* :
//
*//*-----------------------------------------------------------------*/
GRPWIDGET* GRPWIDGETPAGE::FindByName(XCHAR* name)
{
    return FindByName(name, this->root);
}
/*-------------------------------------------------------------------
//   GRPWIDGETPAGE::FindByName
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  22/02/2018 16:49:18
//  @return   GRPWIDGET* :
//
//  @param    XCHAR* :
//  @param    GRPWIDGET* :
//
*//*-----------------------------------------------------------------*/
GRPWIDGET* GRPWIDGETPAGE::FindByName(XCHAR* name, GRPWIDGET* parent)
{
    if (parent->name.Compare(name) == 0)
        return parent;

    for (XDWORD e = 0; e < parent->widgets.GetSize(); e++)
    {
        GRPWIDGET* w = FindByName(name, parent->widgets.Get(e));
        if (w)
            return w;
    }

    return NULL;
}

/*-------------------------------------------------------------------
//   GRPWIDGETPAGE::GetByClass
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  13/02/2018 10:12:39
//  @return   GRPWIDGET* :
//
//  @param    XCHAR* :
//
*//*-----------------------------------------------------------------*/
GRPWIDGET* GRPWIDGETPAGE::GetByClass(XCHAR* name)
{
    return FindByClass(name,root);
}

/*-------------------------------------------------------------------
//   GRPWIDGETPAGE::FindByClass
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  13/02/2018 10:12:46
//  @return   GRPWIDGET* :
//
//  @param    XCHAR* :
//  @param    GRPWIDGET* :
//
*//*-----------------------------------------------------------------*/
GRPWIDGET* GRPWIDGETPAGE::FindByClass(XCHAR* name, GRPWIDGET* parent)
{
    for (XDWORD e=0;e<parent->classes.GetSize();e++)
    if (parent->classes.Get(e)->name.Compare(name) == 0)
        return parent;

    for (XDWORD e = 0; e < parent->widgets.GetSize(); e++)
    {
        GRPWIDGET* w = FindByClass(name, parent->widgets.Get(e));
        if (w)
            return w;
    }

    return NULL;
}


/*-------------------------------------------------------------------
//   GRPWIDGETPAGE::GetScene
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  22/02/2018 16:45:54
//  @return   GRPSCENE* :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
GRPSCENE* GRPWIDGETPAGE::GetScene()
{
    if (ownerpass)
        return ownerpass->GetScene();
    return NULL;
}

/*-------------------------------------------------------------------
//   GRPWIDGETPAGE::Create
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  13/12/2017 12:54:48
//  @return   GRPWIDGET* :
//
//  @param    XCHAR* :
//  @param    XCHAR* :
//  @param    XCHAR* :
//  @param    XCHAR* :
//
*//*-----------------------------------------------------------------*/
GRPWIDGET* GRPWIDGETPAGE::Create(XCHAR* tag, XCHAR* name, XCHAR* classtype, XCHAR* inlinestyle, XCHAR* content)
{
    GRPWIDGET* widget = widgetfactory->Create(tag);

    if (!widget)
        return NULL;

        widget->SetTag(tag);
        widget->SetClassType(classtype);
        widget->SetName(name);
        this->Add(widget);

        AssignStyleToWidget(widget);

        //------------- inline
        widget->scene = this->GetScene();
        widget->node  = this->GetScene()->CreateNode();
        this->processor->Process(&widget->style, inlinestyle);

        //-------------- finish
        widget->content.Set(content);
        widget->text.Set(content);




    return widget;
}

/*-------------------------------------------------------------------
//   GRPWIDGETPAGE::AssignStyleToWidget
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  20/02/2018 14:35:10
//  @return   bool :
//
//  @param    GRPWIDGET* :
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGETPAGE::AssignStyleToWidget(GRPWIDGET* widget)
{
    //------------- classes
    WIDGETSTRING classstring;
    classstring.Set(widget->classtype);

    XVECTOR<WIDGETSTRING*> classes;
    classstring.Explode(__C(' '), &classes);

    for (XDWORD e = 0; e<this->stylesheets.GetSize(); e++)
    {
        GRPSTYLESHEET* sheet = stylesheets.Get(e);

        //--- first * classes
        for (XDWORD i = 0; i < sheet->styles.GetSize(); i++)
        {
            GRPWIDGETSTYLECLASS* styleclass = sheet->styles.Get(i);
            if (styleclass->selector.targettype == GRPWIDGETTARGETTYPE_ALL)
                widget->classes.Add(styleclass);
        }

        //--- second tag types
        for (XDWORD i = 0; i < sheet->styles.GetSize(); i++)
        {
            GRPWIDGETSTYLECLASS* styleclass = sheet->styles.Get(i);
            if (styleclass->selector.targettype == GRPWIDGETTARGETTYPE_TAG)
                if (styleclass->name.Compare(widget->tag) == 0)
                    widget->classes.Add(styleclass);
        }

        //--- third class types
        for (XDWORD i = 0; i < sheet->styles.GetSize(); i++)
        {
            GRPWIDGETSTYLECLASS* styleclass = sheet->styles.Get(i);
            if (styleclass->selector.targettype == GRPWIDGETTARGETTYPE_CLASS)
                for (XDWORD c = 0; c < classes.GetSize(); c++)
                    if (styleclass->name.Compare(classes.Get(c)->Get()) == 0)
                    {
                        widget->classes.Add(styleclass);
                        break;
                    }
        }

        //--- forth id types
        for (XDWORD i = 0; i < sheet->styles.GetSize(); i++)
        {
            GRPWIDGETSTYLECLASS* styleclass = sheet->styles.Get(i);
            if (styleclass->selector.targettype == GRPWIDGETTARGETTYPE_NAME)
                if (styleclass->name.Compare(widget->name.Get()) == 0)
                    widget->classes.Add(styleclass);
        }

        //---- then inline in Process Styles
    }

    classes.DeleteContents();
    classes.DeleteAll();

    return true;
}

/*-------------------------------------------------------------------
//   GRPWIDGETPAGE::FitInterfaceToScreen
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  22/02/2018 13:56:24
//  @return   void :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
void GRPWIDGETPAGE::FitInterfaceToScreen()
{
    //---fit interface to screen
    if (root)
        if (root->widgets.GetSize() > 1)
            this->ownerpass->SetScale(root->widgets.Get(1)->boundingbox.width / this->ownerpass->GetScreen()->GetWidth());
}

/*-------------------------------------------------------------------
//   GRPWIDGETPAGE::CreateProcessor
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  22/02/2018 14:27:43
//  @return   void :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
void GRPWIDGETPAGE::CreateProcessor()
{
    this->processor = new GRPSTYLEPROCESSORCSS();
}

/*-------------------------------------------------------------------
//   GRPWIDGETPAGE::CreateWidgetFactory
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  22/02/2018 16:29:03
//  @return   void :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
void GRPWIDGETPAGE::CreateWidgetFactory()
{
    widgetfactory = new GRPWIDGETFACTORY();
}

/*-------------------------------------------------------------------
//   GRPWIDGETPAGEHTML::Load
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  15/11/2017 16:00:51
//  @return   bool :
//
//  @param    XCHAR* :
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGETPAGEHTML::Load()
{
    XFILEXML2 xmlfile;

    XPATH path;
    path.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, content.Get());

    if (!xmlfile.Open(path))
    {
        XDEBUG_PRINTCOLOR(0, __L("Unable to open page file %s"), content.Get());
        return false;
    }

    if (!xmlfile.ReadAndDecodeAllLines())
    {
        XDEBUG_PRINTCOLOR(0, __L("Parse error on file %s"), content.Get());
        return false;
    }


    XFILEXMLELEMENT* root = xmlfile.GetRoot();
    if (!root) return false;

    GRPSTYLESHEET* sheet = new GRPSTYLESHEET();
    this->Add(sheet);


    this->root = this->Create(root->GetName().Get(), __L(""));
    this->root->root = this->root;
    this->root->boundingbox.width = (GLFLOAT)this->ownerpass->GetScene()->GetScreen()->GetWidth();
    this->root->boundingbox.height = (GLFLOAT)this->ownerpass->GetScene()->GetScreen()->GetHeight();
    this->root->Set(GRPWIDGETSTYLETYPE_WIDTH, (XDWORD)this->root->boundingbox.width);
    this->root->Set(GRPWIDGETSTYLETYPE_HEIGHT, (XDWORD)this->root->boundingbox.height);
    this->root->Set(GRPWIDGETSTYLETYPE_MAX_WIDTH, (XDWORD)this->root->boundingbox.width);
    this->root->Set(GRPWIDGETSTYLETYPE_MAX_HEIGHT, (XDWORD)this->root->boundingbox.height);
    this->root->page = this;
    this->root->ApplyStyles();
    this->root->context = this->root->context;

    for (int e = 0; e < root->GetNElements(); e++)
    {
        XFILEXMLELEMENT* element = root->GetElement(e);
        if (element->GetType() != XFILEXMLELEMENTTYPE_COMMENT)
        {
            ParseElement(element, this->root);
        }
    }


    this->root->Normalize();
    this->root->Align();

    return true;
}


/*-------------------------------------------------------------------
//   GRPWIDGETPAGEHTML::ParseElement
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  15/11/2017 16:45:46
//  @return   bool :
//
//  @param    XFILEXMLELEMENT* :
//  @param    GRPWIDGET* :
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGETPAGEHTML::ParseElement(XFILEXMLELEMENT* element, GRPWIDGET* father)
{
    WIDGETSTRING type = element->GetName();

    if (type.Compare(__L("style")) == 0)
        return ParseStyle(element->GetValue().Get());

    WIDGETSTRING style;
    element->GetValueAttribute(__L("style"), style);
    WIDGETSTRING classes;
    element->GetValueAttribute(__L("class"), classes);

    if (classes.IsEmpty())
        classes.Set(type);
    else
    {
        classes.Insert(__L(" "), 0);
        classes.Insert(type, 0);
    }

    XSTRING name;
    if (!element->GetValueAttribute(__L("name"), name))
         element->GetValueAttribute(__L("id"), name);

    XSTRING text = element->GetValue();

    if (text.GetSize())
    {
        text.DeleteNoCharacters(__L("\r\n\t"), 0, XSTRINGCONTEXT_ALLSTRING);
        ReplaceHTMLEntities(text);
    }

    for (int e = 0; e < element->GetNAttributes(); e++)
    {
        XFILEXMLATTRIBUTE* att=element->GetAttribute(e);
        WIDGETSTRING attname  = att->GetName();
        WIDGETSTRING attvalue = att->GetValue();

        if (attname.Compare(__L("href")) == 0) //has href
        {
            style.AddFormat(__L(";content:url('%s');"), attvalue.Get());
        }
        else
        if (attname.Compare(__L("src")) == 0) //has source
            style.AddFormat(__L(";content:url('%s');"), attvalue.Get());
        else
        if (attname.Compare(__L("colspan")) == 0) //has colspan
            style.AddFormat(__L(";colspan:%s;"), attvalue.Get());
        else
        if (attname.Compare(__L("rowspan")) == 0) //has colspan
            style.AddFormat(__L(";rowspan:%s;"), attvalue.Get());

    }

    GRPWIDGET* widget = this->Create(type.Get(), name.Get(), classes.Get(), style.Get(), text.Get());
    father->Add(widget);

    for (int e = 0; e < element->GetNAttributes(); e++)
    {
        XFILEXMLATTRIBUTE* att = element->GetAttribute(e);
        WIDGETSTRING attname = att->GetName();
        WIDGETSTRING attvalue = att->GetValue();
        widget->attributes[attname.Get()] = attvalue;
    }

    for (int e = 0; e < element->GetNElements(); e++)
    {
        XFILEXMLELEMENT* child = element->GetElement(e);
        if (child->GetType() != XFILEXMLELEMENTTYPE_COMMENT)
        {
            ParseElement(child, widget);
        }
    }
    return true;
}


/*-------------------------------------------------------------------
//   GRPWIDGETPAGEHTML::ReplaceHTMLEntities
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  09/02/2018 12:55:32
//  @return   int :
//
//  @param    XSTRING& :
//
*//*-----------------------------------------------------------------*/
int GRPWIDGETPAGEHTML::ReplaceHTMLEntities(XSTRING& s)
{
    int position = s.FindCharacter(__C('&'));
    if (position != NOTFOUND)
    {
        s.Replace(__L("&euro;"), __L("\u20ac"));
        s.Replace(__L("&aacute;"), __L("\u00e1"));
        s.Replace(__L("&eacute;"), __L("\u00e9"));
        s.Replace(__L("&iacute;"), __L("\u00ed"));
        s.Replace(__L("&oacute;"), __L("\u00f3"));
        s.Replace(__L("&uacute;"), __L("\u00fa"));
        s.Replace(__L("&nacute;"), __L("\u00f1"));
        s.Replace(__L("&Aacute;"), __L("\u00c1"));
        s.Replace(__L("&Eacute;"), __L("\u00c9"));
        s.Replace(__L("&Iacute;"), __L("\u00cd"));
        s.Replace(__L("&Oacute;"), __L("\u00d3"));
        s.Replace(__L("&Uacute;"), __L("\u00da"));
        s.Replace(__L("&amp;"), __L("&"));
        s.Replace(__L("&lt;"), __L("<"));
        s.Replace(__L("&gt;"), __L(">"));
        s.Replace(__L("&quot;"), __L("\""));
        s.Replace(__L("&apos;"), __L("'"));
        s.Replace(__L("&cent;"), __L("\u00a2"));
        s.Replace(__L("&copy;"), __L("\u00a9"));
        s.Replace(__L("&reg;"), __L("\u00ae"));
        s.Replace(__L("&nbsp;"), __L(" "));
    }

    return 0;
}