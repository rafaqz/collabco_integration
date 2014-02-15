<?php
/**
 * @file
 * Demonstraion code for the Collabco Integration API.
 */

/**
 * hook_collabco_integration().
 *
 * A simple way of integrating feature module while keeping them modular, ie
 * they can be turned off without affecting each other or the site aversely.
 * 
 * An array of data provides information on views, page paths and  bundle types
 * provided by a module so the can be used elsewhere without hardcoding
 * dependencies.
 */
function hook_collabco_integration() {
  // Add links to views if a module wants to do that.
  return array(
    'module_name' => array(
      // Entity will be used in add content links and possibly elsewhere.
      'entity' => array(
        'node'=> array(
          'bundle' => array(
            // Provide and add content link for this bundle.
            'add_content_link' => TRUE,
            // Provide a link, path and bundle for the group/user tab.
            // The bundle will be available as the value of the 'integration' context,
            // which will be active if we are on the provided subpath.
            'tab' => array(
              'label' => array(
                'subpath' => 'subpath',
                'node' => 'bundle',
              ),
            ),
          ),
        ),
      ),
      // This view will be altered in a standard way for integrations, removing
      // group or node subscription flags if needed and removing node flag if not.
      // an add content button is is a group aware add content link to the
      // provided node bundle.
      'views' => array(
        'view' => array(
          'display_id' => array(
            'subscribe_flag' => array(
              'group_type' => 'node',
              'entity_type' => 'node',
            ),
            'add_content_link' => array(
              'areas' => array('header','footer'),
            ),
          ),
        ),
      ),
    ),
  );
}

