/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2020 Roberto Fernandez Bautista <roberto.fer.bau@gmail.com>
 * Copyright (C) 2020 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file cadstar_pcb_archive_plugin.cpp
 * @brief Pcbnew PLUGIN for CADSTAR PCB Archive (*.cpa) format: an ASCII format
 *        based on S-expressions.
 */

#include <cadstar_pcb_archive_loader.h>
#include <cadstar_pcb_archive_plugin.h>
#include <class_board.h>
#include <properties.h>


LAYER_MAP CADSTAR_PCB_ARCHIVE_PLUGIN::DefaultLayerMappingCallback(
        const std::vector<INPUT_LAYER_DESC>& aInputLayerDescriptionVector )
{
    LAYER_MAP retval;

    // Just return a the auto-mapped layers
    for( INPUT_LAYER_DESC layerDesc : aInputLayerDescriptionVector )
    {
        retval.insert( { layerDesc.Name, layerDesc.AutoMapLayer } );
    }

    return retval;
}


void CADSTAR_PCB_ARCHIVE_PLUGIN::RegisterLayerMappingCallback(
        LAYER_MAPPING_HANDLER aLayerMappingHandler )
{
    m_layer_mapping_handler       = aLayerMappingHandler;
    m_show_layer_mapping_warnings = false; // only show warnings with default callback
}


CADSTAR_PCB_ARCHIVE_PLUGIN::CADSTAR_PCB_ARCHIVE_PLUGIN()
{
    m_board                       = nullptr;
    m_props                       = nullptr;
    m_layer_mapping_handler       = CADSTAR_PCB_ARCHIVE_PLUGIN::DefaultLayerMappingCallback;
    m_show_layer_mapping_warnings = true;
}


CADSTAR_PCB_ARCHIVE_PLUGIN::~CADSTAR_PCB_ARCHIVE_PLUGIN()
{
}


const wxString CADSTAR_PCB_ARCHIVE_PLUGIN::PluginName() const
{
    return wxT( "CADSTAR PCB Archive" );
}


const wxString CADSTAR_PCB_ARCHIVE_PLUGIN::GetFileExtension() const
{
    return wxT( "cpa" );
}


BOARD* CADSTAR_PCB_ARCHIVE_PLUGIN::Load(
        const wxString& aFileName, BOARD* aAppendToMe, const PROPERTIES* aProperties )
{
    m_props = aProperties;
    m_board = aAppendToMe ? aAppendToMe : new BOARD();

    CADSTAR_PCB_ARCHIVE_LOADER tempPCB(
            aFileName, m_layer_mapping_handler, m_show_layer_mapping_warnings );
    tempPCB.Load( m_board );

    //center the board:
    if( aProperties )
    {
        UTF8 page_width;
        UTF8 page_height;

        if( aProperties->Value( "page_width", &page_width )
                && aProperties->Value( "page_height", &page_height ) )
        {
            EDA_RECT bbbox = m_board->GetBoardEdgesBoundingBox();

            int w = atoi( page_width.c_str() );
            int h = atoi( page_height.c_str() );

            int desired_x = ( w - bbbox.GetWidth() ) / 2;
            int desired_y = ( h - bbbox.GetHeight() ) / 2;

            m_board->Move( wxPoint( desired_x - bbbox.GetX(), desired_y - bbbox.GetY() ) );
        }
    }

    return m_board;
}
