/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2004-2020 KiCad Developers.
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

#include <common.h>
#include <pcb_shape.h>
#include <geometry/seg.h>
#include <geometry/shape_segment.h>
#include <drc/drc_engine.h>
#include <drc/drc_item.h>
#include <drc/drc_rule.h>
#include <drc/drc_test_provider_clearance_base.h>

/*
    Board edge clearance test. Checks all items for their mechanical clearances against the board
    edge.
    Errors generated:
    - DRCE_COPPER_EDGE_CLEARANCE

    TODO:
    - separate holes to edge check
    - tester only looks for edge crossings. it doesn't check if items are inside/outside the board
      area.
    - pad test missing!
*/

class DRC_TEST_PROVIDER_EDGE_CLEARANCE : public DRC_TEST_PROVIDER_CLEARANCE_BASE
{
public:
    DRC_TEST_PROVIDER_EDGE_CLEARANCE () :
            DRC_TEST_PROVIDER_CLEARANCE_BASE()
    {
    }

    virtual ~DRC_TEST_PROVIDER_EDGE_CLEARANCE()
    {
    }

    virtual bool Run() override;

    virtual const wxString GetName() const override
    {
        return "edge_clearance";
    }

    virtual const wxString GetDescription() const override
    {
        return "Tests items vs board edge clearance";
    }

    virtual std::set<DRC_CONSTRAINT_TYPE_T> GetConstraintTypes() const override;

    int GetNumPhases() const override;
};


bool DRC_TEST_PROVIDER_EDGE_CLEARANCE::Run()
{
    m_board = m_drcEngine->GetBoard();

    DRC_CONSTRAINT worstClearanceConstraint;

    if( m_drcEngine->QueryWorstConstraint( DRC_CONSTRAINT_TYPE_EDGE_CLEARANCE,
                                           worstClearanceConstraint, DRCCQ_LARGEST_MINIMUM ) )
    {
        m_largestClearance = worstClearanceConstraint.GetValue().Min();
    }

    reportAux( "Worst clearance : %d nm", m_largestClearance );

    if( !reportPhase( _( "Checking board edge clearances..." ) ) )
        return false;
    
    std::vector<std::unique_ptr<PCB_SHAPE>> boardOutline;
    std::vector<BOARD_ITEM*>                boardItems;

    auto queryBoardOutlineItems =
            [&]( BOARD_ITEM *item ) -> bool
            {
                PCB_SHAPE* shape = static_cast<PCB_SHAPE*>( item );

                if( shape->GetShape() == S_RECT )
                {
                    // A single rectangle for the board would make the RTree useless, so
                    // convert to 4 edges
                    boardOutline.emplace_back( static_cast<PCB_SHAPE*>( shape->Clone() ) );
                    boardOutline.back()->SetShape( S_SEGMENT );
                    boardOutline.back()->SetEndX( shape->GetStartX() );
                    boardOutline.emplace_back( static_cast<PCB_SHAPE*>( shape->Clone() ) );
                    boardOutline.back()->SetShape( S_SEGMENT );
                    boardOutline.back()->SetEndY( shape->GetStartY() );
                    boardOutline.emplace_back( static_cast<PCB_SHAPE*>( shape->Clone() ) );
                    boardOutline.back()->SetShape( S_SEGMENT );
                    boardOutline.back()->SetStartX( shape->GetEndX() );
                    boardOutline.emplace_back( static_cast<PCB_SHAPE*>( shape->Clone() ) );
                    boardOutline.back()->SetShape( S_SEGMENT );
                    boardOutline.back()->SetStartY( shape->GetEndY() );
                    return true;
                }
                else if( shape->GetShape() == S_POLYGON )
                {
                    // Same for polygons
                    SHAPE_LINE_CHAIN poly = shape->GetPolyShape().Outline( 0 );

                    for( size_t ii = 0; ii < poly.GetSegmentCount(); ++ii )
                    {
                        SEG seg = poly.CSegment( ii );
                        boardOutline.emplace_back( static_cast<PCB_SHAPE*>( shape->Clone() ) );
                        boardOutline.back()->SetShape( S_SEGMENT );
                        boardOutline.back()->SetStart( (wxPoint) seg.A );
                        boardOutline.back()->SetEnd( (wxPoint) seg.B );
                    }
                }

                boardOutline.emplace_back( static_cast<PCB_SHAPE*>( shape->Clone() ) );
                boardOutline.back()->SetWidth( 0 );
                return true;
            };

    auto queryBoardGeometryItems =
            [&]( BOARD_ITEM *item ) -> bool
            {
                boardItems.push_back( item );
                return true;
            };

    forEachGeometryItem( { PCB_SHAPE_T }, LSET( Edge_Cuts ), queryBoardOutlineItems );
    forEachGeometryItem( s_allBasicItems, LSET::AllCuMask(), queryBoardGeometryItems );

    wxString val;
    wxGetEnv( "WXTRACE", &val );

    drc_dbg( 2, "outline: %d items, board: %d items\n",
            (int) boardOutline.size(), (int) boardItems.size() );

    for( const std::unique_ptr<PCB_SHAPE>& outlineItem : boardOutline )
    {
        if( m_drcEngine->IsErrorLimitExceeded( DRCE_COPPER_EDGE_CLEARANCE ) )
            break;

        const std::shared_ptr<SHAPE>& refShape = outlineItem->GetEffectiveShape();

        for( BOARD_ITEM* boardItem : boardItems )
        {
            if( m_drcEngine->IsErrorLimitExceeded( DRCE_COPPER_EDGE_CLEARANCE ) )
                break;

            drc_dbg( 10, "RefT %d %p %s %d\n", outlineItem->Type(), outlineItem.get(),
                     outlineItem->GetClass(), outlineItem->GetLayer() );
            drc_dbg( 10, "BoardT %d %p %s %d\n", boardItem->Type(), boardItem,
                     boardItem->GetClass(), boardItem->GetLayer() );

            if ( isInvisibleText( boardItem ) )
                continue;

            const std::shared_ptr<SHAPE>& shape = boardItem->GetEffectiveShape();

            auto constraint = m_drcEngine->EvalRulesForItems( DRC_CONSTRAINT_TYPE_EDGE_CLEARANCE,
                                                              outlineItem.get(), boardItem );

            int      minClearance = constraint.GetValue().Min();
            int      actual;
            VECTOR2I pos;

            accountCheck( constraint );

            if( refShape->Collide( shape.get(), minClearance, &actual, &pos ) )
            {
                std::shared_ptr<DRC_ITEM> drcItem = DRC_ITEM::Create( DRCE_COPPER_EDGE_CLEARANCE );

                m_msg.Printf( drcItem->GetErrorText() + wxS( " " ) + _( "(%s clearance %s; actual %s)" ),
                              constraint.GetName(),
                              MessageTextFromValue( userUnits(), minClearance ),
                              MessageTextFromValue( userUnits(), actual ) );

                drcItem->SetErrorMessage( m_msg );
                drcItem->SetItems( outlineItem->m_Uuid, boardItem->m_Uuid );
                drcItem->SetViolatingRule( constraint.GetParentRule() );

                reportViolation( drcItem, (wxPoint) pos );
            }
        }
    }

    if( !reportPhase( _( "Checking silkscreen to board edge clearances..." ) ) )
        return false;

    boardItems.clear();
    forEachGeometryItem( s_allBasicItems, LSET( 2, F_SilkS, B_SilkS ),
                         queryBoardGeometryItems );

    for( const std::unique_ptr<PCB_SHAPE>& outlineItem : boardOutline )
    {
        if( m_drcEngine->IsErrorLimitExceeded( DRCE_SILK_MASK_CLEARANCE ) )
            break;

        const std::shared_ptr<SHAPE>& refShape = outlineItem->GetEffectiveShape();

        for( BOARD_ITEM* boardItem : boardItems )
        {
            if( m_drcEngine->IsErrorLimitExceeded( DRCE_SILK_MASK_CLEARANCE ) )
                break;

            drc_dbg( 10, "RefT %d %p %s %d\n", outlineItem->Type(), outlineItem.get(),
                     outlineItem->GetClass(), outlineItem->GetLayer() );
            drc_dbg( 10, "BoardT %d %p %s %d\n", boardItem->Type(), boardItem,
                     boardItem->GetClass(), boardItem->GetLayer() );

            if( isInvisibleText( boardItem ) )
                continue;

            const std::shared_ptr<SHAPE>& shape = boardItem->GetEffectiveShape();

            auto constraint = m_drcEngine->EvalRulesForItems( DRC_CONSTRAINT_TYPE_SILK_CLEARANCE,
                                                              outlineItem.get(), boardItem );

            int      minClearance = constraint.GetValue().Min();
            int      actual;
            VECTOR2I pos;

            accountCheck( constraint );

            if( refShape->Collide( shape.get(), minClearance, &actual, &pos ) )
            {
                std::shared_ptr<DRC_ITEM> drcItem = DRC_ITEM::Create( DRCE_SILK_MASK_CLEARANCE );

                if( minClearance > 0 )
                {
                    m_msg.Printf( drcItem->GetErrorText() + wxS( " " ) + _( "(%s clearance %s; actual %s)" ),
                                  constraint.GetName(),
                                  MessageTextFromValue( userUnits(), minClearance ),
                                  MessageTextFromValue( userUnits(), actual ) );

                    drcItem->SetErrorMessage( m_msg );
                }

                drcItem->SetItems( outlineItem->m_Uuid, boardItem->m_Uuid );
                drcItem->SetViolatingRule( constraint.GetParentRule() );

                reportViolation( drcItem, (wxPoint) pos );
            }
        }
    }

    reportRuleStatistics();

    return true;
}


int DRC_TEST_PROVIDER_EDGE_CLEARANCE::GetNumPhases() const
{
    return 1;
}


std::set<DRC_CONSTRAINT_TYPE_T> DRC_TEST_PROVIDER_EDGE_CLEARANCE::GetConstraintTypes() const
{
    return { DRC_CONSTRAINT_TYPE_EDGE_CLEARANCE, DRC_CONSTRAINT_TYPE_SILK_CLEARANCE };
}


namespace detail
{
    static DRC_REGISTER_TEST_PROVIDER<DRC_TEST_PROVIDER_EDGE_CLEARANCE> dummy;
}
