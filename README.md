# Islandora Traditional Knowledge Labels

Generates badges displaying Traditional Knowledge (TK) Labels for Islandora objects with indigenous cultural ties.

## Introduction

Islandora TK Labels provides block containing a TK label image, with a link to the label URI and its details.
It reads the TK Label URI from a Solr field, and builds the HTML from the appropriate assets at localcontexts.org.

TK Labels can be found at https://localcontexts.org/tk-labels/.

The badge will only display on objects with a TK Label URI in some defined element.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Islandora Solr](https://github.com/islandora/islandora_solr)
* [Islandora Badges](../../)

## Installation

Install as usual, see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Configuration path is admin/islandora/tools/badges/tk_labels (Administration > Islandora > Islandora Utility Modules > Islandora Badges Configuration > TK Labels).

* Solr field for TK Labels Project ID: Defaults to dc.identifier. Prefer a multivalued field.
* Image Height: Set the image height in pixels.

## Metadata

You can choose any element for your TK Label Project ID, so long as it creates a Solr field. The default is `dc.identifier`.

The value of the field must be a LocalContexts Project ID. Example from a MODS datastream:

`<identifier type="LocalContexts" displayLabel="LocalContexts Project ID">c44a70ea-d2a3-4198-b967-513167c52f15</accessCondition>`

You will need to create projects for your objects in the [LocalContexts Hub](https://localcontextshub.org) before using this module.

Projects can be created before labels are produced. Labels will automatically be updated via the API when Indigenous groups create them through the Hub.

## Troubleshooting/Issues

Having problems or solved a problem? Check out the Islandora google groups for a solution.

* [Islandora Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora)
* [Islandora Dev Group](https://groups.google.com/forum/?hl=en&fromgroups#!forum/islandora-dev)

## Maintainers/Sponsors

Current maintainers:

* [Brandon Weigel](https://github.com/bondjimbond)

## Development

If you would like to contribute to this module, please check out [CONTRIBUTING.md](CONTRIBUTING.md). In addition, we have helpful [Documentation for Developers](https://github.com/Islandora/islandora/wiki#wiki-documentation-for-developers) info, as well as our [Developers](http://islandora.ca/developers) section on the [Islandora.ca](http://islandora.ca) site.

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)
