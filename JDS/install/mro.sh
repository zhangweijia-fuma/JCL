#!/bin/bash


# MRO
psql -U postgres -d JamesSoft -c "create schema mro"
psql -U postgres -d JamesSoft -f $SRVDIR/sql/mro/component_property_list
psql -U postgres -d JamesSoft -f $SRVDIR/sql/mro/component_type.sql
psql -U postgres -d JamesSoft -f $SRVDIR/sql/mro/device_list.sql
psql -U postgres -d JamesSoft -f $SRVDIR/sql/mro/device_type.sql
psql -U postgres -d JamesSoft -f $SRVDIR/sql/mro/mro_config.sql
psql -U postgres -d JamesSoft -f $SRVDIR/sql/mro/mro_system_data.sql
psql -U postgres -d JamesSoft -f $SRVDIR/sql/mro/property_type.sql
psql -U postgres -d JamesSoft -f $SRVDIR/sql/mro/vendor_check_history.sql
psql -U postgres -d JamesSoft -f $SRVDIR/sql/mro/vendor_list.sql
