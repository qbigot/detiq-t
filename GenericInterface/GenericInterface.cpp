/*
 * Copyright 2011-2012 Benoit Averty, Samuel Babin, Matthieu Bergere, Thomas Letan, Sacha Percot-Tétu, Florian Teyssier
 * 
 * This file is part of DETIQ-T.
 * 
 * DETIQ-T is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * DETIQ-T is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with DETIQ-T.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "GenericInterface.h"

using namespace std;
using namespace genericinterface;

inline void initResource_res() { Q_INIT_RESOURCE(res); }

GenericInterface::GenericInterface(QString name, Qt::DockWidgetArea navPos) : _nbServices(3)
{
    initResource_res();
    addService(FILE_SERVICE, new FileService);
    addService(WINDOW_SERVICE, new WindowService(navPos));
    addService(UTILITY_SERVICE, new UtilityService);

    this->setWindowIcon(QIcon(":/images/image-x-generic.png"));

    if (name != "")
        setWindowTitle(name);
}

int GenericInterface::addService(Service* s)
{
	int id = this->_nbServices++;
	this->addService(id, s);
	return id;
}

void GenericInterface::addService(int id, Service* s) throw (BadIdException)
{
  if (_services.find(id) == _services.end())
  {
    _services[id] = s;
  }
  else
    throw new BadIdException(id);
}

void GenericInterface::changeService(int id, Service* s) throw (BadIdException)
{
  if (_services.find(id) == _services.end())
    throw new BadIdException(id);
  else
  {
    delete _services[id];
    _services[id] = s;
  }
}

Service* GenericInterface::getService(int id) throw (BadIdException)
{
  if (_services.find(id) == _services.end())
    throw BadIdException(id);
  else
    return _services[id];
}

WindowService* GenericInterface::windowService()
{
    WindowService* ws = dynamic_cast<WindowService*>(_services[WINDOW_SERVICE]);
    return ws;
}

FileService* GenericInterface::fileService()
{
    FileService* fs = dynamic_cast<FileService*>(_services[FILE_SERVICE]);
    return fs;
}

int GenericInterface::addNewService(Service* s)
{
	int id = this->_nbServices++;
    _services[id] = s;
    try
    {
      s->display(this);
      s->connect(this);
    }
    catch (ServiceConnectionException e)
    {
      Log::info(e.what());
      QMessageBox::critical(this, 
      tr("Interface's integrity compromised"),
      tr("An error occurred in the addition of a new service. The interface may not offer all the expected features. See the log file for more informations."));
    }
    
	return id;
}

void GenericInterface::run(bool shw)
{
  bool fail(false);

  // Call the method display() of all the services
  for (map<int, Service*>::iterator it = _services.begin() ; it != _services.end() ; ++it)
  {
    try
    {
      (*it).second->display(this);
    }
    catch (ServiceConnectionException e)
    {
      Log::info(e.what());
      fail = true;
    }
  }
  // Call the method connect() of all the services
  for (map<int, Service*>::iterator it = _services.begin() ; it != _services.end() ; ++it)
  {
    try
    {
      (*it).second->connect(this);
    }
    catch (ServiceConnectionException e)
    {
      Log::info(e.what());
      fail = true;
    }
  }

  if (fail)
  {
      QMessageBox::critical(this, 
      tr("Interface's integrity compromised"),
      tr("An error occurred in the addition of a service. The interface may not offer all the expected features. See the log file for more informations."));
  }

  // Now that everything is added and connected, we add the last elements
  this->finalizeInterface();

  if (shw) this->show();
}

QMdiArea* GenericInterface::initCentralWidget()
{
  static bool init(false);
  QMdiArea* res;

  if (!init)
  {
    init = true;
    res = new QMdiArea;
    setCentralWidget(res);
  }
  else
    throw new AlreadyInitException;

  return res;
}

QMenu* GenericInterface::menu(QString name)
{
    QMenu* res;

    if (_menus.find(name) != _menus.end())
    {
        res = _menus[name];
    }
    else
    {
        if(_menus.find(tr("&Window")) != _menus.end()) {
            res = new QMenu(name, this);
            menuBar()->insertMenu(_menus[tr("&Window")]->menuAction(), res);
        }
        else {
            res = menuBar()->addMenu(name);
        }
        _menus[name] = res;
    }

    return res;
}

QToolBar* GenericInterface::toolBar(QString name)
{
    QToolBar* res;

    if (_toolBars.find(name) != _toolBars.end())
    {
        res = _toolBars[name];
    }
    else
    {
        res = addToolBar(name);
        _toolBars[name] = res;
    }

    return res;
}

void GenericInterface::finalizeInterface()
{
  // Add the Exit action
  this->menu(tr("&File"))->addSeparator();
  QAction* actionExit = this->menu(tr("&File"))->addAction(tr("&Exit"));
  QObject::connect(actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));

  /*QAction* actionQt = this->menu("?")->addAction("About Qt");
  QObject::connect(actionQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));*/
}

