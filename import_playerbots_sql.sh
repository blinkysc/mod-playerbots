#!/bin/bash

# Import mod-playerbots SQL files into the characters database
# Usage: ./import_playerbots_sql.sh [mysql_user] [mysql_password] [database_name]

MYSQL_USER="${1:-acore}"
MYSQL_PASS="${2:-acore}"
DB_NAME="${3:-acore_characters}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SQL_BASE="$SCRIPT_DIR/data/sql"

echo "==================================="
echo "mod-playerbots SQL Import Script"
echo "==================================="
echo "Database: $DB_NAME"
echo "User: $MYSQL_USER"
echo ""

# Check if mysql is available
if ! command -v mysql &> /dev/null; then
    echo "ERROR: mysql command not found. Please install MySQL client."
    exit 1
fi

# Function to import SQL file
import_sql() {
    local file="$1"
    if [ -f "$file" ]; then
        echo "Importing: $(basename "$file")"
        if mysql -u"$MYSQL_USER" -p"$MYSQL_PASS" "$DB_NAME" < "$file" 2>&1; then
            return 0
        else
            echo "  WARNING: Error importing $(basename "$file")"
            return 1
        fi
    fi
}

echo "Step 1: Importing playerbots base tables..."
echo "-------------------------------------------"

# Import playerbots base tables in order (version table first)
if [ -d "$SQL_BASE/playerbots/base" ]; then
    # Version table first
    import_sql "$SQL_BASE/playerbots/base/version_db_playerbots.sql"

    # Then all other base tables
    for sql_file in "$SQL_BASE/playerbots/base/"*.sql; do
        if [[ "$(basename "$sql_file")" != "version_db_playerbots.sql" && "$(basename "$sql_file")" != "updates_include.sql" && "$(basename "$sql_file")" != "updates.sql" ]]; then
            import_sql "$sql_file"
        fi
    done
else
    echo "WARNING: playerbots/base directory not found"
fi

echo ""
echo "Step 2: Importing character name tables..."
echo "-------------------------------------------"

# Import character-related tables
if [ -d "$SQL_BASE/characters/base" ]; then
    for sql_file in "$SQL_BASE/characters/base/"*.sql; do
        import_sql "$sql_file"
    done
else
    echo "WARNING: characters/base directory not found"
fi

echo ""
echo "Step 3: Importing world database tables (into acore_world)..."
echo "--------------------------------------------------------------"

WORLD_DB="${DB_NAME/characters/world}"
if [ -d "$SQL_BASE/world/base" ]; then
    for sql_file in "$SQL_BASE/world/base/"*.sql; do
        echo "Importing to $WORLD_DB: $(basename "$sql_file")"
        mysql -u"$MYSQL_USER" -p"$MYSQL_PASS" "$WORLD_DB" < "$sql_file" 2>&1 || echo "  WARNING: Error importing $(basename "$sql_file")"
    done
else
    echo "WARNING: world/base directory not found"
fi

echo ""
echo "==================================="
echo "Import complete!"
echo "==================================="
echo ""
echo "If you see errors above, you may need to:"
echo "1. Check your MySQL credentials"
echo "2. Ensure the database exists"
echo "3. Run as: ./import_playerbots_sql.sh <user> <password> <db_name>"
