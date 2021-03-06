/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2012-2014 Miguel Angel Ajo <miguelangel@nbee.es>
 * Copyright (C) 1992-2014 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @file dialog_scripting.cpp
 */


#include <Python.h>
#undef HAVE_CLOCK_GETTIME  // macro is defined in Python.h and causes redefine warning

#include <pcb_edit_frame.h>
#include <dialog_scripting.h>


DIALOG_SCRIPTING::DIALOG_SCRIPTING( wxWindow* parent )
    : DIALOG_SCRIPTING_BASE( parent )
{
    SetFocus();

}



void DIALOG_SCRIPTING::OnRunButtonClick( wxCommandEvent& event )
{
	wxCharBuffer buffer = m_txScript->GetValue().ToUTF8();
    int          retv   = PyRun_SimpleString( buffer.data()) ;

    if( retv != 0 )
        wxLogError( "Python error %d occurred running command:\n\n`%s`", retv, buffer );
}
