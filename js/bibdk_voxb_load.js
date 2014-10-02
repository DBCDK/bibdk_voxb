(function($) {

  Drupal.bibdkSetRating = function(voxb) {
    if(voxb.error) {
      $('.bibdk_voxb_tab[data-pid=' + voxb.pid + ']').prepend(voxb.error);
    }
    else {
      $('.bibdk_voxb_tab[data-pid=' + voxb.pid + ']').html(voxb.markup);
      // attach behaviours to stay in context
      Drupal.attachBehaviors($('.bibdk_voxb_tab[data-pid=' + voxb.pid + ']'));
      Drupal.voxb_settings.init(voxb);
    }
  }

  Drupal.bibdkGetRating = function(div) {
    div.find('.rating').once().last().append('<span class="ajax-progress" style="padding-left:2em; margin-top:-3px"><span class="throbber"></span></span>');
    var pid = $(div).attr('data-pid');
    var request = $.ajax({
      url: Drupal.settings.basePath + 'voxb/ajax/get_rating',
      type: 'POST',
      data: {
        pid: pid
      },
      dataType: 'json',
      success: Drupal.bibdkSetRating
    });
  }

  Drupal.voxbUpdateRating = function(link) {
    var href = Drupal.settings.basePath + link.attr('href');
    // find the div holding the link
    var div = link.closest('.bibdk_voxb_tab');
    // show a throbber

    // do an ajax call. No response is expected
    $.get(href, function(data) {
      if(data.error) {
        data.pid = $(div).attr('data-pid');
        Drupal.bibdkSetRating(data);
      }
      else {
        // update after request is completed
        Drupal.bibdkGetRating(div);
      }
    });
  };


  Drupal.voxb_settings = {
    init: function() {
      // add mouseover
      $('.voxb-rating.rate-enabled .star').mouseover(function(e) {
        $(this).addClass('star-hover');
      });
      // add mouseout
      $('.voxb-rating.rate-enabled .star').mouseout(function(e) {
        $(this).removeClass('star-hover');
      });
      // add click
      $('.voxb-rating.rate-enabled .rating').click(function(e) {
        e.preventDefault();
        Drupal.voxbUpdateRating($(this));
      });
      if(typeof(voxb_update_review_response) != 'undefined') {
        response = voxb_update_review_response;
        Drupal.voxb_review_set_message(response.selector, response.info);
        voxb_update_review_response = 'undefined';
      }
    }
  };

  Drupal.behaviors.bibdk_voxb_load = {
    attach: function(context) {
      $('.reviews', context).click(function(e) {
        var href = $(this).attr('href');
        var voxb_tab = $(href).find('.bibdk_voxb_tab');
        Drupal.bibdkGetRating(voxb_tab);
      });
    }
  };

  Drupal.voxb_review_set_message = function(selector,message){
    $(selector).html(message);
  }

  var voxb_update_review_response;

  Drupal.voxb_review_update = function(ajax,response) {
    var div = $(response.selector);
    var tab = div.closest('.worktabs');
    var rev = tab.find('.reviews').first();
    var href = $(rev).attr('href');
    var voxb_tab = $(href).find('.bibdk_voxb_tab');

    voxb_update_review_response = response;

    Drupal.bibdkGetRating(voxb_tab);

    //Drupal.voxb_review_set_message(response.selector, response.info);
  };

  Drupal.voxb_offensive_posted = function(ajax, response) {
    $(response.selector).after(response.info);
  };

  Drupal.voxb_review_set_error = function (ajax,response) {
    var voxb_tab = $('.bibdk_voxb_tab[data-pid=' + response.pid + ']');
    var div = voxb_tab.find('.bibdk-write-review-link').first();
    div.html(response.info);
  }

  // add refresh function to drupal commands
  $(function() {
    Drupal.ajax.prototype.commands.bibdk_voxb_offensive_posted = Drupal.voxb_offensive_posted;
    Drupal.ajax.prototype.commands.bibdk_voxb_review_saved = Drupal.voxb_review_update;
    Drupal.ajax.prototype.commands.bibdk_voxb_review_error = Drupal.voxb_review_set_error;
  });
}
  (jQuery));

